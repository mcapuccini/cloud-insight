package se.uu.it.cinsight

import scala.collection.mutable.HashMap
import scala.math.ceil
import scala.math.log
import scala.math.pow
import scala.math.sqrt
import scala.util.Random

import org.apache.spark.SparkContext
import org.apache.spark.mllib.linalg.Vector
import org.apache.spark.mllib.linalg.Vectors
import org.apache.log4j.Logger

import se.uu.it.easymr.EasyMapReduce
import org.apache.spark.rdd.RDD

/**
 * Main engine
 *
 * @constructor create a new engine with parameters and model file
 * @param prior prior distribution of the parameters
 * @param beta confidence level of the test
 * @param u number of particles per populations
 * @param epsilon tolerance schedule
 * @param model filename of the model
 * @param data flow cytometry data set (sequence of pairs (time, cytometry data))
 */
class CloudInsight(
    val prior: List[(Double, Double)],
    val beta: Double,
    val U: Int,
    val epsilon: Seq[Double],
    val model: String,
    val M: Int,
    @transient val sc: SparkContext) extends Serializable {
  val alpha = 1 - sqrt(1 - beta)
  val T = epsilon.length
  val S = for (eps <- epsilon) yield (
    ceil(-log(alpha / 2)
      / (2 * pow(eps - sqrt(-log(alpha / 2) / (2 * M)), 2))).toInt)

  var t = 1
  var particles = List[RDD[(List[Double], Double)]]()
  @transient lazy val logg = Logger.getLogger(getClass.getName)

  def perturbation_distribution(theta: List[Double], theta_star: List[Double], eps: Double): Double = {
    return (for ((value, i) <- theta.zipWithIndex) yield if (theta_star(i) * (1 - eps) <= value && value <= theta_star(i) * (1 + eps))
      1.0 / (theta_star(i) * (1 + eps) - theta_star(i) * (1 - eps)).abs else 0.0).product
  }

  /**
   * Samples one candidate parameter point according to prior distribution
   *     and previously accepted points.
   *
   * @return a new parameter point
   */
  def sample_candidate(partial_sum: List[(List[Double], Double)]): List[Double] = {
    t match {
      case 1 => {
        for (bounds <- prior) yield Random.nextDouble * (bounds._2 - bounds._1) + bounds._1
      }
      case _ => {
        for (
          value <- partial_sum.filter(_._2 > Random.nextDouble).head._1
        ) yield value * (Random.nextDouble * 2 * epsilon(t - 1) + 1 - epsilon(t - 1))
      }
    }
  }

  /**
   * Compute the weight of the given particle
   *
   * @param particle particle whose weight must be computed
   */
  def compute_weight(particle: List[Double], particles_t2: List[(List[Double], Double)]): Double = {
    var weight = if (t == 1) 1.0 else (for ((value, i) <- particle.zipWithIndex) yield if (prior(i)._1 <= value && value <= prior(i)._2)
      1.0 / (prior(i)._2 - prior(i)._1) else 0.0).product

    return if (t > 1 && weight > 0.0) weight /
      (for ((old_particle, old_weight) <- particles_t2) yield old_weight * perturbation_distribution(particle, old_particle, epsilon(t - 1))).sum
    else weight
  }

  /**
   * Evaluate particle using the sequential solver
   *
   * @param particles List of Vectors of particles
   * @param sc number of simulations to perform
   * @param tol tolerance for the acceptance
   * @return was the particle accepted
   */
  def evaluate_particle(particle: RDD[Vector], sims: Int, tol: Double) = {

    if (model != "birthdeath") {
      throw new NotImplementedError
    }

    val stringrdd = particle.map(_.toJson)
    
    val particles = new EasyMapReduce(stringrdd)
      .map(
        imageName = "mcapuccini/insight-particle-evaluator",
        command =
          "/opt/INSIGHT/bin/INSIGHTv3" +
            " --problem_file /var/example_data/BirthDeath/problem_birthdeath.xml" +
            " -N " + sims.toString +
            " -t " + tol.toString())

    particles.getRDD.map { boolStr =>
      if (boolStr == "1") {
        true
      } else {
        false
      }
    }

  }

  /**
   * Run the INSIGHT algorithm
   *
   * @return sampled posterior distribution of the parameter space
   */
  def run() = {
    
    // Get default parallelism
    val defaultParallelism =
      sc.getConf.get("spark.default.parallelism", "2").toInt
    
    var exit_criterion = false
    while (t <= T && !exit_criterion) {
      var count_rejected_particles = 0L     
      var accepted_particles = sc.emptyRDD[List[Double]]
      var accepted_particles_count = 0L
      while (accepted_particles_count < U && !exit_criterion) {
        val batchSize = if(accepted_particles_count+count_rejected_particles !=0) {
          ((U-accepted_particles_count)*1.15/(1.0*accepted_particles_count/(accepted_particles_count+count_rejected_particles))).toInt
        } else {
          U
        }

        val partial_sum = if(t>1){
          val particles_t2 = particles(t-2).collect
          particles_t2.scanLeft((List[Double](), 0.0))(
            (p1, p2) => (p2._1, p1._2 + p2._2))
          .map({ case (p, w) => (p, w / particles(t - 2).map(_._2).sum) }).toList
        }
        else{
          List[(List[Double], Double)]()
        }

        val batch = sc.parallelize(1 to batchSize, defaultParallelism).map { _ =>
          Vectors.dense(sample_candidate(partial_sum).toArray)
        }  
        var is_accepted = evaluate_particle(batch, S(t-1), epsilon(t-1))  
        accepted_particles ++= batch.zip(is_accepted).filter(_._2).map(_._1.toArray.toList)
        count_rejected_particles += batch.count - batch.zip(is_accepted).filter(_._2).count
        logg.info("t: "+t.toString()+" epsilon: "+epsilon(t-1)+" S: "+S(t-1)+" accepted_particles_length: "+accepted_particles_count.toString()+" acceptance_rate: "+(1.0*accepted_particles_count/(accepted_particles_count+count_rejected_particles)).toString())
        if(accepted_particles_count==0)
          exit_criterion=true;
        accepted_particles_count = accepted_particles.count
      }
      val particles_t2 = if(t>1) particles(t-2).collect.toList else List[(List[Double], Double)]()

      accepted_particles = accepted_particles.zipWithIndex.filter(_._2 < U).map(_._1)
      //compute weights
      particles ++= List(accepted_particles.map(particle => (particle, compute_weight(particle, particles_t2))))
      t += 1
    }
    particles
  }
}

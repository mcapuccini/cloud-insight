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
    val sc: SparkContext) {
  val alpha = 1 - sqrt(1 - beta)
  val T = epsilon.length
  val S = for (eps <- epsilon) yield (
    ceil(-log(alpha / 2)
      / (2 * pow(eps - sqrt(-log(alpha / 2) / (2 * M)), 2))).toInt)

  var t = 1
  var particles = List[List[(List[Double], Double)]]()
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
  def sample_candidate(): List[Double] = {
    t match {
      case 1 => {
        for (bounds <- prior) yield Random.nextDouble * (bounds._2 - bounds._1) + bounds._1
      }
      case _ => {
        for (
          value <- particles(t - 2).scanLeft((List[Double](), 0.0))(
            (p1, p2) => (p2._1, p1._2 + p2._2))
            .map({ case (p, w) => (p, w / particles(t - 2).map(_._2).sum) })
            .filter(_._2 > Random.nextDouble).head._1
        ) yield value * (Random.nextDouble * 2 * epsilon(t - 1) + 1 - epsilon(t - 1))
      }
    }
  }

  /**
   * Compute the weight of the given particle
   *
   * @param particle particle whose weight must be computed
   */
  def compute_weight(particle: List[Double]): Double = {
    var weight = if (t == 1) 1.0 else (for ((value, i) <- particle.zipWithIndex) yield if (prior(i)._1 <= value && value <= prior(i)._2)
      1.0 / (prior(i)._2 - prior(i)._1) else 0.0).product

    return if (t > 1 && weight > 0.0) weight /
      (for ((old_particle, old_weight) <- particles(t - 2)) yield old_weight * perturbation_distribution(particle, old_particle, epsilon(t - 1))).sum
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
  def evaluate_particle(particle: List[Vector], sims: Int, tol: Double): Seq[Boolean] = {

    if (model != "birthdeath") {
      throw new NotImplementedError
    }

    val defaultParallelism =
      sc.getConf.get("spark.default.parallelism", "2").toInt

    val rdd = sc.parallelize(particle, defaultParallelism)

    val stringrdd = rdd.map(_.toJson)

    var cmd = ""

    // The serial solver will by default read the particle list from /input and write it back to /output
    // This is how the serial shell call looks like:
    //cmd = "../INSIGHT/INSIGHTv3 --problem_file ../example_data/BirthDeath/problem_birthdeath.xml -N "+sc.toString+" -t "+tol.toString();
    //cmd.!!

    val particles = new EasyMapReduce(stringrdd)
      .mapPartitions(
        imageName = "mcapuccini/insight-particle-evaluator",
        command =
          "/opt/INSIGHT/bin/INSIGHTv3" +
            " --problem_file /var/example_data/BirthDeath/problem_birthdeath.xml" +
            " -N " + sims.toString +
            " -t " + tol.toString())

    particles.getRDD.collect().map { boolStr =>
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
  def run(): Seq[Iterable[(Seq[Double], Double)]] = {
    var exit_criterion = false
    while (t <= T && !exit_criterion) {
      var count_accepted_particles = 0
      var count_rejected_particles = 0     
      var accepted_particles = List[List[Double]]()
      while (accepted_particles.length < U && !exit_criterion) {
        var batch = (for (u <- 1 to
            (if(count_accepted_particles+count_rejected_particles !=0)
              ((U-accepted_particles.length)*1.15/(1.0*count_accepted_particles/(count_accepted_particles+count_rejected_particles))).toInt
              else U))
          yield Vectors.dense(sample_candidate().toArray)).toList
        var is_accepted = evaluate_particle(batch, S(t-1), epsilon(t-1))
        accepted_particles ++= batch.zip(is_accepted).filter(_._2).map(_._1.toArray.toList)
        count_accepted_particles = accepted_particles.length
        count_rejected_particles += batch.length - batch.zip(is_accepted).filter(_._2).map(_._1.toArray.toList).length
        logg.info("t: "+t.toString()+" epsilon: "+epsilon(t-1)+" S: "+S(t-1)+" accepted_particles_length: "+accepted_particles.length.toString()+" acceptance_rate: "+(1.0*count_accepted_particles/(count_accepted_particles+count_rejected_particles)).toString())
        if(accepted_particles.length==0)
          exit_criterion=true;
      }
      accepted_particles = accepted_particles.take(U)
      //compute weights
      particles ++= List(for (particle <- accepted_particles) yield (particle, compute_weight(particle)))
      t += 1
    }
    return particles
  }
}

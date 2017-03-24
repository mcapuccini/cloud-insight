package uu.se.it.cinsight

import scala.collection.mutable.HashMap
import scala.math.ceil
import scala.math.log
import scala.math.pow
import scala.math.sqrt
import scala.util.Random

import org.apache.spark.SparkContext
import org.apache.spark.mllib.linalg.Vector

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
    val data: Seq[(Double, Iterable[Double])]) {
  val alpha = 1 - sqrt(1 - beta)
  val T = epsilon.length
  val M = (for ((time, values) <- data) yield values.size).min
  val S = for (eps <- epsilon) yield (
    ceil(-log(alpha / 2)
      / (2 * pow(eps - sqrt(-log(alpha / 2) / (2 * M)), 2))).toInt)

  var t = 1
  var particles = List[List[(List[Double], Double)]]()

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
          value <- particles.apply(t - 2).scanLeft((List[Double](), 0.0))(
            (p1, p2) => (p2._1, p1._2 + p2._2))
            .map({ case (p, w) => (p, w / particles.apply(t - 2).map(_._2).sum) })
            .filter(_._2 > Random.nextDouble).head._1
        ) yield value * (Random.nextDouble * 2 * epsilon.apply(t - 1) + 1 - epsilon.apply(t - 1))
      }
    }
  }

  /**
   * Evaluate particle using the sequential solver
   *
   * @param particles List of Vectors of particles
   * @param sc number of simulations to perform
   * @param tol tolerance for the acceptance
   * @return was the particle accepted
   */
  def evaluate_particle(particle: List[Vector], sims: Int, tol: Double, sc: SparkContext): Seq[Boolean] = {

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
        imageName = "insight-particle-evaluator",
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
   * Performs sc simulations with given parameter theta
   *
   * @param theta parameter set to use for these simulations
   * @param sc number of simulations to perform
   * @return simulated flow cytometry data according to theta
   */
  def run_simulation(theta: HashMap[String, Double], sc: Int): Seq[Iterable[Double]] = {
    throw new NotImplementedError
  }

  /**
   * Measures Kolmogorov distance between two data sets
   *
   * Each datasets consists of a list of sets of values (one set per
   * timepoint). x and y should have the same length.
   *
   * @param x first dataset
   * @param y second dataset
   * @return the distance between the two datasets
   */
  def kolmogorov_distance(x: Seq[Iterable[Double]], y: Seq[Iterable[Double]]): Double = {
    assert(x.length == y.length)
    x.zip(y).map({
      case (a, b) =>
        (a.map((_, b.size)) ++ b.map((_, -a.size))) // Merge experimental and simulated datasets
          .groupBy(_._1).map({ case (k, v) => (k, v.map(_._2).sum) }).toList // Aggregate duplicates
          .sortBy(_._1).map(_._2).scanLeft(0.0)(_ + _) // Compute cumulative sum
          .map(_.abs).max / (a.size * b.size)
    }).max
  }

  /**
   * Run the INSIGHT algorithm
   *
   * @return sampled posterior distribution of the parameter space
   */
  def run(): Seq[Iterable[HashMap[String, Double]]] = {
    throw new NotImplementedError
  }
}

package uu.se.it.cinsight

import scala.collection.mutable.HashMap
import scala.math.ceil
import scala.math.log
import scala.math.pow
import scala.math.sqrt

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
    val prior: HashMap[String, (Double, Double)],
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

  /**
   * Samples one candidate parameter point according to prior distribution
   *     and previously accepted points.
   *
   * @return a new parameter point
   */
  def sample_candidate(): HashMap[String, Double] = {
    throw new NotImplementedError
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

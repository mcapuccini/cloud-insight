package uu.se.it.cinsight
import scala.collection.mutable.HashMap

/** Main engine
  *
  * @constructor create a new engine with parameters and model file
  * @param prior prior distribution of the parameters
  * @param beta confidence level of the test
  * @param u number of particles per populations
  * @param epsilon tolerance schedule
  * @param model filename of the model
  * @param data flow cytometry data set
  */
class CloudInsight
    (
        prior: (HashMap[String, Double]) => Double,
        beta: Double,
        u: Int,
        epsilon: Seq[Double],
        model: String,
        data: Seq[(Double, Iterable[Double])])
{
    /** Samples one candidate parameter point according to prior distribution
      *     and previously accepted points.
      *
      * @return a new parameter point
      */
    def sample_candidate() : HashMap[String, Double] = {
      return HashMap[String, Double]()
    }

    /** Performs sc simulations with given parameter theta
      *
      * @param theta parameter set to use for these simulations
      * @param sc number of simulations to perform
      * @return simulated flow cytometry data according to theta
      */
    def run_simulation(theta: HashMap[String, Double], sc: Int) : Seq[Iterable[Double]] = {
      return Seq[Iterable[Double]]()
    }

    /** Measures Kolmogorov distance between two data sets
      *
      * Each datasets consists of a list of sets of values (one set per
      * timepoint). x and y should have the same length.
      *
      * @param x first dataset
      * @param y second dataset
      * @return the distance between the two datasets
      */
    def kolmogorov_distance(x: Seq[Iterable[Double]], y: Seq[Iterable[Double]]) : Double = {
      assert(x.length == y.length)
      x.zip(y).map({case (a, b) =>
                 (a.map((_, b.size)) ++ b.map((_, -a.size))) // Merge experimental and simulated datasets
                 .groupBy(_._1).map({case (k, v) => (k, v.map(_._2).sum)}).toList // Aggregate duplicates
                 .sortBy(_._1).map(_._2).scanLeft(0.0)(_ + _) // Compute cumulative sum
                 .map(_.abs).max/(a.size*b.size)
        }).max
    }

    /** Run the INSIGHT algorithm
      *
      * @return sampled posterior distribution of the parameter space
      */
    def run() : Seq[Iterable[HashMap[String, Double]]] = {
      return Seq[Iterable[HashMap[String, Double]]]()
    }
}

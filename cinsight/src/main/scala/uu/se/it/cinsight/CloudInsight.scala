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
      * @param x first dataset
      * @param y second dataset
      * @return the distance between the two datasets
      */
    def kolmogorov_distance(x: Seq[Iterable[Double]], y: Seq[Iterable[Double]]) : Double = {
      return 0.toDouble
    }
}

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

}

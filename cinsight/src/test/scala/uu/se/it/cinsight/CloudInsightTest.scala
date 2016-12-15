package uu.se.it.cinsight

import org.junit.runner.RunWith
import org.apache.spark.SharedSparkContext
import org.scalatest.FunSuite
import org.scalatest.junit.JUnitRunner
import org.scalatest.BeforeAndAfter

import scala.collection.immutable.List
import scala.collection.mutable.HashMap

@RunWith(classOf[JUnitRunner])
class CloudInsightTest extends FunSuite with SharedSparkContext with BeforeAndAfter {
  
    var engine: CloudInsight = _
    
    before {
       engine = new CloudInsight(
          (x: HashMap[String, Double]) => 0.0,
          0.1,
          10,
          List(1.0, 0.1, 0.01),
          "model.xml",
          List((1.0, List(0.0, 1.0, 2.0)), (2.0, List(10.0, 11.0, 10.0))))
    }
    test("Basic Kolmogorov distances are measured properly") {
        assert(engine.kolmogorov_distance(List(List(0.0)), List(List(10.0))) === 1.0)
        assert(engine.kolmogorov_distance(List(List(-10.0)), List(List(10.0))) === 1.0)

        assert(engine.kolmogorov_distance(List(List(0.0)), List(List(0.0))) === 0.0)
        assert(engine.kolmogorov_distance(List(List(1.0)), List(List(1.0))) === 0.0)
        assert(engine.kolmogorov_distance(List(List(-1.0)), List(List(-1.0))) === 0.0)
        assert(engine.kolmogorov_distance(List(List(1.0, 0.0, -1)), List(List(0.0, -1, 1))) === 0.0)
        assert(engine.kolmogorov_distance(List(List(1.0, -1)), List(List(0.0))) === 0.5)
        assert(engine.kolmogorov_distance(List(List(1.0, 0.0)), List(List(-1.0))) === 1.0)

        assert(engine.kolmogorov_distance(List(List(0.0), List(1.0)), List(List(10.0), List(1.0))) === 1.0)
        assert(engine.kolmogorov_distance(List(List(0.0), List(1.0)), List(List(10.0), List(-1.0))) === 1.0)

        assert(engine.kolmogorov_distance(List(List(0.0), List(1.0)), List(List(0.0), List(1.0))) === 0.0)
    }
}

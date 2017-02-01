package uu.se.it.cinsight

import org.junit.runner.RunWith
import org.apache.spark.SharedSparkContext
import org.scalatest.FunSuite
import org.scalatest.junit.JUnitRunner
import org.scalatest.BeforeAndAfter

import scala.collection.immutable.List
import scala.collection.mutable.HashMap

import scala.math._

@RunWith(classOf[JUnitRunner])
class CloudInsightTest extends FunSuite with SharedSparkContext with BeforeAndAfter {
  
    var engine: CloudInsight = _
    
    before {
        engine = new CloudInsight(
            (x: HashMap[String, Double]) => 0.0,
            0.05,
            10,
            List(0.1, 0.09, 0.06, 0.04),
            "model.xml",
            List((1.0, List.fill(1000)(List(0.0, 1.0, 2.0, 1.0, 0.5)).flatten),
                 (2.0, List.fill(1000)(List(10.0, 11.0, 10.0, 12.0, 11.1)).flatten),
                 (3.0, List.fill(1500)(List(20.0, 21.0, 30.0, 52.0, 12.3)).flatten))
        )
    }
    test("Engine construction"){
        assert(engine.alpha === 1-sqrt(1-0.05))
        assert(engine.T === 4)
        assert(engine.M === 5000)
        assert(engine.S === List(350, 458, 1430, 5991))

        assert(engine.t === 1)
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

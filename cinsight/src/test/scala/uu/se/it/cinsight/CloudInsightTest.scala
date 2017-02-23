package uu.se.it.cinsight

import scala.collection.immutable.List
import scala.collection.mutable.HashMap
import scala.math.sqrt

import org.apache.spark.SharedSparkContext
import org.junit.runner.RunWith
import org.scalatest.BeforeAndAfter
import org.scalatest.FunSuite
import org.scalatest.junit.JUnitRunner

@RunWith(classOf[JUnitRunner])
class CloudInsightTest extends FunSuite with SharedSparkContext with BeforeAndAfter {

  var engine1: CloudInsight = _
  var engine2: CloudInsight = _

  before {
    engine1 = new CloudInsight(
      HashMap[String, (Double, Double)]("alpha" -> (1.0, 2.0),
        "beta" -> (10.0, 15.0)),
      0.05,
      10,
      List(0.1, 0.09, 0.06, 0.04),
      "birthdeath",
      List((1.0, List.fill(1000)(List(0.0, 1.0, 2.0, 1.0, 0.5)).flatten),
        (2.0, List.fill(1000)(List(10.0, 11.0, 10.0, 12.0, 11.1)).flatten),
        (3.0, List.fill(1500)(List(20.0, 21.0, 30.0, 52.0, 12.3)).flatten)))

    engine2 = new CloudInsight(
      HashMap[String, (Double, Double)]("alpha" -> (1.0, 2.0),
        "beta" -> (10.0, 15.0)),
      0.05,
      10,
      List(0.1, 0.0),
      "birthdeath",
      List((1.0, List.fill(1000)(List(0.0, 1.0, 2.0, 1.0, 0.5)).flatten),
        (2.0, List.fill(1000)(List(10.0, 11.0, 10.0, 12.0, 11.1)).flatten),
        (3.0, List.fill(1500)(List(20.0, 21.0, 30.0, 52.0, 12.3)).flatten)))
  }
  test("Engine construction") {
    assert(engine1.alpha === 1 - sqrt(1 - 0.05))
    assert(engine1.T === 4)
    assert(engine1.M === 5000)
    assert(engine1.S === List(350, 458, 1430, 5991))

    assert(engine1.t === 1)
  }
  test("Basic Kolmogorov distances are measured properly") {
    assert(engine1.kolmogorov_distance(List(List(0.0)), List(List(10.0))) === 1.0)
    assert(engine1.kolmogorov_distance(List(List(-10.0)), List(List(10.0))) === 1.0)

    assert(engine1.kolmogorov_distance(List(List(0.0)), List(List(0.0))) === 0.0)
    assert(engine1.kolmogorov_distance(List(List(1.0)), List(List(1.0))) === 0.0)
    assert(engine1.kolmogorov_distance(List(List(-1.0)), List(List(-1.0))) === 0.0)
    assert(engine1.kolmogorov_distance(List(List(1.0, 0.0, -1)), List(List(0.0, -1, 1))) === 0.0)
    assert(engine1.kolmogorov_distance(List(List(1.0, -1)), List(List(0.0))) === 0.5)
    assert(engine1.kolmogorov_distance(List(List(1.0, 0.0)), List(List(-1.0))) === 1.0)

    assert(engine1.kolmogorov_distance(List(List(0.0), List(1.0)), List(List(10.0), List(1.0))) === 1.0)
    assert(engine1.kolmogorov_distance(List(List(0.0), List(1.0)), List(List(10.0), List(-1.0))) === 1.0)

    assert(engine1.kolmogorov_distance(List(List(0.0), List(1.0)), List(List(0.0), List(1.0))) === 0.0)
  }
  test("New candidates are sampled properly") {
    engine2.particles = List(
      for (i <- List.range(0, engine2.U)) yield (engine2.sample_candidate(), 1.0))
    for (particle <- engine2.particles.head) {
      assert(particle._1.apply("alpha") <= 2.0 && particle._1.apply("alpha") >= 1.0)
      assert(particle._1.apply("beta") <= 15.0 && particle._1.apply("beta") >= 10.0)
    }

    engine2.particles = List(
      for (i <- List.range(0, engine2.U)) yield if (i == 5) (HashMap[String, Double]("alpha" -> 1.0), 1.0)
      else (HashMap[String, Double]("alpha" -> 2.0), 0.0))
    engine2.t += 1
    engine2.particles :+= {
      for (i <- List.range(0, engine2.U)) yield (engine2.sample_candidate(), 1.0)
    }
    for (particle <- engine2.particles.tail.head) {
      assert(particle._1.apply("alpha") === 1.0)
    }

  }
  test("Particle is accepted") {

    var tol=10
    var sc=100
    var particle= HashMap[String, Double]("birth" -> 2.0,
        "death" -> 2.0)
	
    assert(engine1.evaluate_particle(particle,sc,tol) === true)

  }
  test("Particle is not accepted") {

    var tol=0.2;
    var sc=100;
    var particle= HashMap[String, Double]("birth" -> 2.0,
        "death" -> 2.0)
	
    assert(engine1.evaluate_particle(particle,sc,tol) === false)

  }
}

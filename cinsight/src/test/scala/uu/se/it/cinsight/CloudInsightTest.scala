package uu.se.it.cinsight

import scala.collection.immutable.List
import scala.math.sqrt

import org.apache.spark.SharedSparkContext
import org.apache.spark.mllib.linalg.Vectors
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
      List[(Double, Double)]((1.0, 2.0), (10.0, 15.0)),
      0.05,
      10,
      List(0.1, 0.09, 0.06, 0.04),
      "birthdeath",
      List((1.0, List.fill(1000)(List(0.0, 1.0, 2.0, 1.0, 0.5)).flatten),
        (2.0, List.fill(1000)(List(10.0, 11.0, 10.0, 12.0, 11.1)).flatten),
        (3.0, List.fill(1500)(List(20.0, 21.0, 30.0, 52.0, 12.3)).flatten)),
      sc)

    engine2 = new CloudInsight(
      List[(Double, Double)]((1.0, 2.0), (10.0, 15.0)),
      0.05,
      10,
      List(0.1, 0.1, 0.0),
      "birthdeath",
      List((1.0, List.fill(1000)(List(0.0, 1.0, 2.0, 1.0, 0.5)).flatten),
        (2.0, List.fill(1000)(List(10.0, 11.0, 10.0, 12.0, 11.1)).flatten),
        (3.0, List.fill(1500)(List(20.0, 21.0, 30.0, 52.0, 12.3)).flatten)),
      sc)
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
      assert(particle._1.apply(0) <= 2.0 && particle._1.apply(0) >= 1.0)
      assert(particle._1.apply(1) <= 15.0 && particle._1.apply(1) >= 10.0)
    }

    engine2.particles = List(
      for (i <- List.range(0, engine2.U)) yield if (i == 5) (List[Double](1.0), 1.0)
      else (List[Double](2.0), 0.0))
    engine2.particles ++= engine2.particles
    engine2.t += 2
    engine2.particles :+= {
      for (i <- List.range(0, engine2.U)) yield (engine2.sample_candidate(), 1.0)
    }
    for (particle <- engine2.particles(2)) {
      assert(particle._1(0) === 1.0)
    }

  }
  test("Weights are computed properly") {
    assert(engine2.compute_weight(List(1.0, 1.0)) === 1.0)
    assert(engine2.compute_weight(List(0.0, 0.0)) === 1.0)

    engine2.particles = List(
      for (i <- List.range(0, engine2.U)) yield (List(1.5, 12.5), 1.0))
    engine2.t = 2
    assert((engine2.compute_weight(List(1.5, 12.5)) - 1.0 / (5 * 10 / (1.5 * 0.2 * 12.5 * 0.2))).abs <= 0.0000001)
    assert(engine2.compute_weight(List(0.0, 0.0)) === 0.0)
  }
  test("Particle is accepted") {

    var tol = 10
    var sim = 100
    var particles = List(
      Vectors.dense(2.0, 2.0),
      Vectors.dense(3.0, 1.0),
      Vectors.dense(4.0, 3.0),
      Vectors.dense(5.0, 9.0))

    assert(engine1.evaluate_particle(particles, sim, tol, sc) === Seq(true, true, true, true))

  }
  test("Particle is not accepted") {

    var tol = 0.2;
    var sim = 100;
    var particles = List(
      Vectors.dense(2.0, 2.0),
      Vectors.dense(3.0, 1.0),
      Vectors.dense(4.0, 3.0),
      Vectors.dense(5.0, 9.0))

    assert(engine1.evaluate_particle(particles, sim, tol, sc) === List(false, false, false, false))

  }
}

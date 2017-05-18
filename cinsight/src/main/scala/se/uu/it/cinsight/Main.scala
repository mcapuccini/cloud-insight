package se.uu.it.cinsight

import org.apache.spark.SparkConf
import org.apache.spark.SparkContext
import scopt.OptionParser
import java.io._
import org.json4s._
import org.json4s.JsonDSL._
import org.json4s.jackson.JsonMethods._

object Main {

  case class Params(
    master: String = null,
    U: Int = 100,
    epsilon_up: Double = 0.7,
    epsilon_coeff: Double = 0.5,
    epsilon_n: Int = 10
    )

  def main(args: Array[String]) = {

    val defaultParams = Params()

    val parser = new OptionParser[Params]("Main") {
      opt[String]("master")
        .text("spark master")
        .action((x, c) => c.copy(master = x))
      opt[Int]("U")
        .text("number of particles")
        .action((x, c) => c.copy(U = x))
      opt[Double]("epsilon_up")
        .text("epsilon upper bound")
        .action((x, c) => c.copy(epsilon_up = x))
      opt[Double]("epsilon_coeff")
        .text("epsilon coefficient")
        .action((x, c) => c.copy(epsilon_coeff = x))
      opt[Int]("epsilon_n")
        .text("number of epsilons")
        .action((x, c) => c.copy(epsilon_n = x))
    }

    parser.parse(args, defaultParams).map { params =>
      run(params)
    } getOrElse {
      sys.exit(1)
    }

  }

  def run(params: Params) {

    val conf = new SparkConf()
      .setAppName("CloudINSIGHT")
    if (params.master != null) {
      conf.setMaster(params.master)
    }
    val sc = new SparkContext(conf)
    
    val epslist = (0 to params.epsilon_n-1).map(params.epsilon_up*math.pow(params.epsilon_coeff, _))

    // Experiment goes here
    var engine = new CloudInsight(
      List[(Double, Double)]((0.0, 10.0), (0.0, 2.0)), // prior (birth, death)
      0.05, // beta
      params.U, // U
      epslist, // epsilon
      "birthdeath",
      80000,
      sc)

    val particles = engine.run()
    val particles_writer = new PrintWriter(new File("particles.json"))
    particles_writer.write(compact(render(
        (for (batch <- particles) yield (for (particle <- batch) yield particle._1.toList).toList).toList)))
    particles_writer.close
    
    val eps_writer = new PrintWriter(new File("epsilon.json"))
    eps_writer.write(compact(render(
        engine.epsilon)))
    eps_writer.close

    sc.stop

  }

}

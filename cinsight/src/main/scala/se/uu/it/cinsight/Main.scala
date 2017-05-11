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
    master: String = null)

  def main(args: Array[String]) = {

    val defaultParams = Params()

    val parser = new OptionParser[Params]("Main") {
      opt[String]("master")
        .text("spark master")
        .action((x, c) => c.copy(master = x))
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

    // Experiment goes here
    var engine = new CloudInsight(
      List[(Double, Double)]((0.0, 10.0), (0.0, 2.0)), // prior (birth, death)
      0.05, // beta
      10, // U
      List(0.7, 0.6, 0.5), // epsilon
      "birthdeath",
      8000,
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

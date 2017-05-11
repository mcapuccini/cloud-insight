package se.uu.it.cinsight

import org.apache.spark.SparkConf
import org.apache.spark.SparkContext
import scopt.OptionParser

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

    sc.stop

  }

}
package se.uu.it.cinsight

import org.apache.spark.SparkConf
import org.apache.spark.SparkContext

object Main {
  
  def main(args: Array[String]) = {
    
    val conf = new SparkConf()
      .setAppName("CloudINSIGHT")
    val sc = new SparkContext(conf)

    // Experiment goes here
    
    sc.stop
    
  }

}
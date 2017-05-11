# cloudINSIGHT
[![Build Status](https://travis-ci.org/mcapuccini/cloud-insight.svg?branch=master)](https://travis-ci.org/mcapuccini/cloud-insight)

## Prerequisites
- Install Docker and make sure that your user has the rights to run Docker
- Pull `insight-particle-evaluator` Docker container:
```
docker pull mcapuccini/insight-particle-evaluator
```
- Make sure that `TMPDIR` is defined in your environmemt (e.g. `TMPDIR=/tmp`)
- Install Maven
- Install git

## Build instructions
- Clone this repo and locate into it:
```
git clone https://github.com/mcapuccini/cloud-insight.git
cd cloud-insight
```
- Build with Maven
```
cd cinsight && mvn clean package
```

## Run in local mode
1) Download Apache Spark, extract it to a directory and set ``export SPARK_HOME=/your/spark/directory``
2) Run the analysis locally
```
$SPARK_HOME/bin/spark-submit \
  --class se.uu.it.cinsight.Main
  --master local[*]
  cinsight/target/cinsight-0.1.0-SNAPSHOT-jar-with-dependencies.jar
```

## Submit to a cluster
1) Make sure that `SPARK_HOME` is set properly in the machine that you are using to submit the application
2) Submit to a Spark cluster
```
$SPARK_HOME/bin/spark-submit \
  --class se.uu.it.cinsight.Main
  --master spark://spark-url:7077
  cinsight/target/cinsight-0.1.0-SNAPSHOT-jar-with-dependencies.jar
```
> **Note:** when using TheSparkBox the `spark-url` is `localhost`

## Run from Eclipse (Scala IDE)
1) Right click on Main.scala > Run as > Scala application. Let it fail.
2) Right click on Main.scala > Run as > Run configurations > Arguments > type `--master local` in the program arguments field. Apply and run. 

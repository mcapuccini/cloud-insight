# cloudINSIGHT
[![Build Status](https://travis-ci.org/mcapuccini/cloud-insight.svg?branch=master)](https://travis-ci.org/mcapuccini/cloud-insight)

## Run locally (Ubuntu)
0) Install dependencies:
  - Install Docker and make sure your user in into Docker group
  - Download Apache Spark, extract it to a directory and set ``export SPARK_HOME=/your/spark/directory``
1) Pull `insight-particle-evaluator` Docker container
```
docker pull mcapuccini/insight-particle-evaluator
```
2) Make sure that `TMPDIR` is defined in your environmemt (e.g. `TMPDIR=/tmp`)
3) Build cloudINSIGHT
```
cd cinsight && mvn clean package
```
4) Run the analysis locally
```
$SPARK_HOME/bin/spark-submit \
  --class se.uu.it.cinsight.Main
  --master local[*]
  cinsight/target/cinsight-0.1.0-SNAPSHOT-jar-with-dependencies.jar
```

# cloudINSIGHT
[![Build Status](https://travis-ci.org/mcapuccini/cloud-insight.svg?branch=master)](https://travis-ci.org/mcapuccini/cloud-insight)

## Run locally (Ubuntu)
1) Build docker container
```
cd container
sudo docker build -t insight-particle-evaluator .
```
2) Add Enviroment variable to your run configuration
- Right click on CloudInsight.scala -> RunAs -> Run Configurations -> Enviroment
- Create Variable TMPDIR with value /tmp

3) Add your user into the docker usergroup
http://askubuntu.com/questions/477551/how-can-i-use-docker-without-sudo

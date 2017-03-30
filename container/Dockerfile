FROM debian:jessie
RUN apt-get update -y && apt-get install libboost-all-dev build-essential  -y 
ADD INSIGHT /opt/INSIGHT
ADD example_data /var/example_data
RUN cd /opt/INSIGHT && ./install_script_sequential.sh

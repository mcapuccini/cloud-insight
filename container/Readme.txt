Installation : 

Boost needs to be installed. To use the parallel version of INSIGHT, mpi has to be installed too. 
1. Download the code. 
2. Change your current directory to INSIGHT
	cd ./INSIGHT
3. Several options are available:
	1. Run "sh install_script_sequential.sh" (possibly need root privileges) to install just the sequential version of INSIGHT into the current directory. This will create a bin folder in the current directory with a binary "INSIGHTv3".
	2. Run "sh install_script_mpi.sh" (possibly need root privileges) to install the parallel and the sequential version of INSIGHT into the current directory. This will create a bin folder in the current directory with a binary "INSIGHTv3" and a parallel binary "INSIGHTv3_parallel".
	3. Run "./configure". When the option "--enable-mpi" is added the sequential as well as the parallel version is getting installed. The installation path can be changed by changing the option "--prefix=/new/installation/path" where "/new/installation/path" is the path to the new installation location.


Running the examples:

In the folder "example_data" three examples are provided. To run those examples you need to run the INSIGHT binary providing the the path to one of the problem files as first argument. Change to the INSIGHT binary directory and type for instance "bin/INSIGHTv3 ../../example_data/BirthDeath/problem_birthdeath.xml" to run the sequential version of INSIGHT with the birth-death model, without fluorescence. 

/*
 ============================================================================
 Name        : 	INSIGHTv3.cpp
 Author      : 	Jan Mikelson
 Version     :
 Copyright   : 	This free software is available under the Creative Commons Attribution Share Alike License.
 You are permitted to use, redistribute and adapt this software as long as appropriate credit
 is given to the original author, and all derivative works are distributed under the same
 license or a compatible one.
 For more information, visit http://creativecommons.org/licenses/by-sa/3.0/ or send a letter to
 Creative Commons, 171 2nd Street, Suite 300, San Francisco, California, 94105, USA.
 Description : 	Sequential version of the INSIGHT algorithm. Further details can be found in Lillacci & Khammash 2013
 ============================================================================
 */

#include <ctime>
#include <vector>
#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include <map>

#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

char cCurrentPath[FILENAME_MAX];

#include <boost/program_options.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/ref.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <Eigen/Dense>

#include "INSIGHTv3.h"
#include "BirthDeathModel.h"
#include "InsightAlgorithm.h"
#include "InsightParticleSamplerFactory.h"
#include "IllegalArgumentException.h"
#include "ParticleEvaluatorInsight.h"
#include "UniformSamplerFactory.h"
#include "ScaledUniformSamplerFactory.h"
#include "ProblemFileReader.h"
#include "ModelDescription.h"
#include "UniformSampler.h"
#include "InsightIteration.h"
#include "PrevPopReader.h"

using namespace INSIGHTv3;
using boost::property_tree::ptree;
using boost::property_tree::read_json;

//Default values
const static std::string PROBLEM_FILE = "";
const static int S = 3;
const static double FINAL_TOLERANCE = 0.03;
const static int NUM_PARTICLES = 1000;
const static std::string OUTPUT_FILE = "";
const static std::string PREVIOUS_POPULATION_FILE = "";
const static std::string PARTICLE_INPUT = "/input";
const static std::string PARTICLE_OUTPUT = "/output";
const static bool IGNORE_WEIGHTS = false;
const static bool VERBOSE = false;
const static int PRINT = 20;
const static double BETA = 0.01;
const static double TOL_KOLMOGOROV = 0.0000001;
const static double KAPPA_KOLMOGOROV = 0.005224; // kappa value for M = 80000 and beta = 0.01
const static double TOL_KAPPA_KOLMOGOROV = 0.01;
const static double PARTICLE_TOLERANCE = 1.0;
const static int NUM_TRAJECTORIES = 10;

std::string problem_file_name;
int s;
double final_tolerance;
int num_particles;
std::string output_file_name;
std::string prev_pop_file;
bool ignore_weights;
bool verbose; // surporess alloutput
int print;
double beta;
double tolerance_kolmogorov;
double kappa_kolmogorov;
double tol_kappa_kolmovorog;
double particle_tolerance;
std::string particle_in;
std::string particle_out;
int num_trajectories;

namespace po = boost::program_options;

void handleOptions(int argc, char * argv[]);
void printInfo(ModelDescription& desc);

int main(int argc, char *argv[]) {

	handleOptions(argc, argv);

	clock_t tic = clock();
	int seed = time(NULL);
#ifdef DEBUG
	seed = 2;
#endif
	RngPtr rng = boost::make_shared<RandomNumberGenerator>(seed);

	ProblemFileReader reader;
	ModelDescription desc = reader.read(rng, problem_file_name);

	printInfo(desc);
	writer_ptr writer = boost::make_shared<InsightOutputWriter>(
			output_file_name);

	KolmogorovComputerPtr kolmogorov_computer = boost::make_shared<
			KolmogorovComputer>(*desc.original_data, beta, tolerance_kolmogorov,
			kappa_kolmogorov);
	sampler_factory_ptr factory =
			boost::make_shared<ScaledUniformSamplerFactory>(boost::ref(rng),
					desc.model->num_params, *desc.lower_bounds_parameter,
					*desc.upper_bounds_parameter, ignore_weights);

	sampler_ptr prior_sampler = boost::make_shared<UniformSampler>(
			boost::ref(rng), *desc.lower_bounds_parameter,
			*desc.upper_bounds_parameter);
	evaluator_ptr evaluator = boost::make_shared<ParticleEvaluatorInsight>(
			boost::ref(*desc.model), *desc.times, boost::ref(rng),
			kolmogorov_computer); // need to use boost::ref, see boost

	InsightIteration iteration(prior_sampler);
	ToleranceProviderPtr tolerance_provider = boost::make_shared<
			FixedSequenceToleranceProvider>(s, desc.model->model,
			kolmogorov_computer);

	sampler_ptr first_sampler;
	std::vector<double> prev_acceptance_rates;
	std::vector<double> prev_tolerances;
	std::vector<int> prev_num_simulations;


	//prev_num_simulations.push_back(20); // set to number of iterations

	if (prev_pop_file.length() > 0) {
		PrevPopReader prev_pop_reader;
		ParticleSet particle_set;
		prev_pop_reader.readPreviousPop(prev_pop_file, desc.model->num_params,
				&particle_set, &prev_acceptance_rates, &prev_tolerances, &prev_num_simulations);
		first_sampler = factory->createSampler(particle_set,
				kolmogorov_computer->getThresholdForS(s), s);
	} else {
		first_sampler = prior_sampler;
	}
	InsightAlgorithm algorithm(iteration, factory, evaluator, writer,
			tolerance_provider, final_tolerance, num_particles,
			desc.model->num_params, prev_acceptance_rates, prev_tolerances, prev_num_simulations);
	IterationLogger logger(print);

	//algorithm.run(first_sampler, logger);

	algorithm._updateTolerance(&num_trajectories, &particle_tolerance, &logger);


    bool acceptable = 0;
    parameters particle(desc.model->num_params);
    //first_sampler->sampleParticle(&particle);

//open particle input file
std::ifstream infile (particle_in.c_str());

//open particle output file
std::ofstream outfile (particle_out.c_str());
if (!outfile.is_open())
    return -1; // error!

std::string line;
int particle_count=0;

while (std::getline(infile, line)) // parse particles one by one
{

    //using text file
    unsigned first = line.find("[");
    unsigned last = line.find("]");
    std::string strNew = line.substr (first+1,last-first);

    std::istringstream iss(strNew);
    char ch;

    double number=2.0;
    int pcount=0;
    while (pcount<desc.model->num_params) {
        iss >> number;
        iss>>ch; //flush the ,
        particle(pcount)=number;
        pcount++;
    }

    if(verbose)
        std::cout << "particle " << particle_count << ": " << particle << std::endl;

    acceptable = evaluator->isParticleAcceptable(particle);
    if(acceptable)
        outfile << "1\n";
    else
        outfile << "0\n";
}
outfile.close();

	clock_t toc = clock();
	if(verbose) {
        std::cout << "\n\nSMC inference complete." << std::endl;
        std::cout << "\nElapsed time: " << ((double) (toc - tic)) / CLOCKS_PER_SEC
                << "sec." << std::endl;
	}
	return 0;
}

void handleOptions(int argc, char * argv[]) {

	po::options_description desc("Allowed options");
    desc.add_options()("help", "produce help message")("problem_file",
            po::value<std::string>(&problem_file_name)->default_value(
                    PROBLEM_FILE),
            "problem file. A problem file must always be provided!")(
            "number_simulations,s", po::value<int>(&s)->default_value(S),
            "starting number of simulations")("final tolerance,e",
            po::value<double>(&final_tolerance)->default_value(FINAL_TOLERANCE),
            "the final tolerance for the algorithm")("pop_size,n",
            po::value<int>(&num_particles)->default_value(NUM_PARTICLES),
            "define the population size")("output_file,O",
            po::value<std::string>(&output_file_name)->default_value(
                    OUTPUT_FILE), "output file")("previous_pop_file,P",
            po::value<std::string>(&prev_pop_file)->default_value(
                    PREVIOUS_POPULATION_FILE), "previous population file")(
            "ignore_weights,i",
            po::value<bool>(&ignore_weights)->default_value(IGNORE_WEIGHTS),
            "ignore weights")("print,p",
            po::value<int>(&print)->default_value(PRINT),
            "define the frequency of print outs")("beta,b",
            po::value<double>(&beta)->default_value(BETA),
            "the confidence of the kolmogorov test for the rejection rule")(
            "tol_kol,k",
            po::value<double>(&tolerance_kolmogorov)->default_value(
                    TOL_KOLMOGOROV),
            "the tolerance for the algorithm to compute the inverse of the kolmogorov distribution")(
            "kappa,K",
            po::value<double>(&kappa_kolmogorov)->default_value(
                    KAPPA_KOLMOGOROV),
            "Kappa for the computation of the Kolmogorov distance")(
            "tol_kol_kap,T",
            po::value<double>(&tol_kappa_kolmovorog)->default_value(
                    TOL_KAPPA_KOLMOGOROV),
            "the tolerance for the algorithm to compute kappa for the computation of the Kolmogorov distance")("X,X",
                    po::value<std::string>(&particle_in)->default_value(PARTICLE_INPUT),"Particle input file")("o,o",
                    po::value<std::string>(&particle_out)->default_value(PARTICLE_OUTPUT),"Particle output file")("t,t",
		po::value<double>(&particle_tolerance)->default_value(
             PARTICLE_TOLERANCE),"Particle tolerance")("N,N",po::value<int>(&num_trajectories)->default_value(
                     NUM_TRAJECTORIES),"Number of trajectories")("v,v",po::value<bool>(&verbose)->default_value(
                             VERBOSE),"Verbose");


	po::positional_options_description p;
	p.add("problem_file", -1);

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::store(
			po::command_line_parser(argc, argv).options(desc).positional(p).run(),
			vm);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << desc << "\n";
		abort();
	}

	if (problem_file_name.size() == 0) {
		std::cerr << "Problem file is required" << std::endl;
		abort();
	}

	if (output_file_name.size() == 0) {
		GetCurrentDir(cCurrentPath, sizeof(cCurrentPath));
		output_file_name = std::string(cCurrentPath) + "/resutls.txt";
	}

}

void printInfo(ModelDescription& desc) {

    if(verbose) {
        std::cout << "\nThis is INSIGHT v3.0\n" << std::endl;
        std::cout << "Problem file path:" << std::endl;
        std::cout << problem_file_name << std::endl;
        std::cout << "\nThe new particle population will be saved in:" << std::endl;
        std::cout << output_file_name << std::endl;

        std::cout << "\nWill estimate " << desc.model->model_name << " using "
                << num_particles << " SMC particles." << std::endl;
        std::cout << "\nThe data is assumed to have been created using "
                << desc.model->num_outputs << " outputs," << std::endl;
        std::cout << "measured at " << desc.times->size() << " timepoints."
                << std::endl;

        if (prev_pop_file.length() > 0) {
            std::cout << "A previous population is loaded from file: " << std::endl;
            std::cout << prev_pop_file << std::endl;
        }
        std::cout << "To estimate the density of each population "
                << "Kernel Density Estimation with uniform Kernel" << std::endl;
        if (ignore_weights) {
            std::cout << std::endl << "Particle weights will be ignored by sampler!"
                    << std::endl;
        }
        std::cout << "is used." << std::endl;
        std::cout << "\nThe sequential version of the algorithm is used."
                << std::endl << std::endl;
    }
}

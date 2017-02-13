/*
 * PrevPopReader.cpp
 *
 *  Created on: Sep 28, 2012
 *      Author: jan
 *
 *      This class reads populations of particles from previous runs. It is assumed that a particle file is provided and a weight file, where
 *      the name of the weight file is the particle file name with a _weights appended. For instance: results.txt and results_weights.txt
 */

#include <fstream>
#include <typeinfo>
#include <iostream>

#include "PrevPopReader.h"
#include "IOUtils.h"
#include "DataFormatException.h"

namespace INSIGHTv3 {

PrevPopReader::PrevPopReader() {

}

PrevPopReader::~PrevPopReader() {

}

void PrevPopReader::readPreviousPop(std::string pop_file, int num_params,
		ParticleSet* particle_set) {

	if (!pop_file.empty()) {
		std::string weight_file = IOUtils::appendToFileName(pop_file,
				"weights");

		std::ifstream p_file(pop_file.c_str());
		std::ifstream w_file(weight_file.c_str());

		if (!p_file.is_open()) {
			throw DataFormatException(
					(typeid(this).name()
							+ std::string(
									" could not load previous particle population, since the particle file be opened\n")).c_str());
		}
		if (!w_file.is_open()) {
			std::cerr << "Weight file " << weight_file.c_str()
					<< " could not be opened! Uniform weights are used!"
					<< std::endl;
		}
		parameters particle(num_params);
		double value;
		double weight;
		while (p_file >> value) {
			particle(0) = value;
			if (!w_file.is_open()) {
				weight = 1;
			} else {
				w_file >> weight;
			}

			for (int param_nbr = 1; param_nbr < num_params; param_nbr++) {

				if (p_file.eof()) {
					throw DataFormatException(
							(typeid(this).name()
									+ std::string(
											" could not load previous particle population, since the number of parameters seems not to work out\n")).c_str());
				}

				p_file >> value;
				particle(param_nbr) = value;
			}

			particle_set->acceptParticle(particle, weight);
		}

		p_file.close();
		w_file.close();
	}
}

void PrevPopReader::readPreviousPop(std::string pop_file, int num_params,
		ParticleSet* particle_set, std::vector<double>* prev_acceptance_rates,
		std::vector<double>* prev_tolerances,
		std::vector<int>* prev_num_simulations) {

	readPreviousPop(pop_file, num_params, particle_set);

	std::string prev_pop_loc = IOUtils::getFileLocation(pop_file);
	readPrevAcceptanceRates(prev_pop_loc + "/results_acceptance_rates.txt",
			prev_acceptance_rates, prev_tolerances, prev_num_simulations);

}
void PrevPopReader::readPrevAcceptanceRates(std::string acceptance_rate_file,
		std::vector<double>* prev_acceptance_rates,
		std::vector<double>* prev_tolerances,
		std::vector<int>* prev_num_simulations) {
	if (!acceptance_rate_file.empty()) {

		std::ifstream rate_file(acceptance_rate_file.c_str());

		if (!rate_file.is_open()) {
			std::cerr << "Precious acceptance rate file "
					<< acceptance_rate_file.c_str()
					<< " could not be opened and will be ignored!" << std::endl;
		}

		double acceptance_rate;
		double tolerance;
		int num_simulations;
		while (rate_file >> num_simulations) {
			rate_file >> tolerance;
			rate_file >> acceptance_rate;

			prev_num_simulations->push_back(num_simulations);
			prev_tolerances->push_back(tolerance);
			prev_acceptance_rates->push_back(acceptance_rate);
		}

		rate_file.close();
	}

}
} /* namespace INSIGHTv3 */

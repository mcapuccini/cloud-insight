/*
 * OutputWriter.cpp
 *
 *  Created on: Oct 10, 2012
 *      Author: jan
 *
 *      This class writes the results into a file. It produces an output file for the particles and an output file for the weights.
 *      It also produces a txt file with the acceptance rates once the algorithm is finished.
 *
 */

#include "InsightOutputWriter.h"

#include <stdio.h>
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>

#include "INSIGHTv3.h"
#include "IOUtils.h"

namespace INSIGHTv3 {

InsightOutputWriter::InsightOutputWriter(std::string outputFileName) :
		_output_file_name(outputFileName) {
}

InsightOutputWriter::~InsightOutputWriter() {

}

void InsightOutputWriter::writeOutput(const ParticleSet& particle_set,
		size_t s) {
	writeOutput(particle_set, s, "");
}

void InsightOutputWriter::writeOutput(const ParticleSet& particle_set, size_t s,
		std::string name_suffix) {
	std::stringstream ss;
	ss << s;

	std::string outputFileWithSuffix = _output_file_name;
	if (name_suffix.length() > 0) {
		outputFileWithSuffix = IOUtils::appendToFileName(_output_file_name,
				name_suffix);
	}
	std::string outputFile = IOUtils::appendToFileName(outputFileWithSuffix,
			ss.str());

	std::string outputFileWeights = IOUtils::appendToFileName(outputFile,
			"weights");

	const std::vector<parameters>& particles = particle_set.getParticles();
	const std::vector<double>& weights = particle_set.getWeights();

	writeParticles(particles, outputFile);
	writeWeights(weights, outputFileWeights);
}

void InsightOutputWriter::writeParticles(
		const std::vector<parameters>& particles, std::string outputFile) {

	std::ofstream data_file(outputFile.c_str());
	if (!data_file.is_open()) {
		std::cerr << "error opening file " << outputFile.c_str()
				<< "for writing particles.. Particles could not be saved!!"
				<< std::endl << std::endl;
		return;
	}

	for (size_t i = 0; i < particles.size(); i++) {
		for (int j = 0; j < (particles[i]).size(); j++) {
			double part = (particles[i])(j);
			data_file << part << std::endl;
		}
	}
	data_file.close();
	std::cout << "current particles wrote into " << outputFile.c_str()
			<< std::endl;
}

void InsightOutputWriter::writeWeights(const std::vector<double>& weights,
		std::string outputFileWeights) {

	std::ofstream wt_file(outputFileWeights.c_str());
	if (!wt_file.is_open()) {
		std::cerr << "error opening file " << outputFileWeights.c_str()
				<< "for writing weights.. Weights could not be saved!!"
				<< std::endl << std::endl;
		return;
	}

	for (size_t k = 0; k < weights.size(); k++) {
		wt_file << weights[k] << std::endl;
	}
	wt_file.close();
	std::cout << "current weights wrote into " << outputFileWeights.c_str()
			<< std::endl;
}

void InsightOutputWriter::writeAcceptanceRate(std::vector<int>& num_simulations,
		std::vector<double>& tolerances,
		std::vector<double>& acceptance_rates) {

	std::string outputFileWithSuffix = _output_file_name;
	std::string outputFile = IOUtils::appendToFileName(outputFileWithSuffix,
			"acceptance_rates");

	writeAcceptanceRate(num_simulations, tolerances, acceptance_rates,
			outputFile);
}

void InsightOutputWriter::writeAcceptanceRate(std::vector<int>& num_simulations,
		std::vector<double>& tolerances, std::vector<double>& acceptance_rates,
		std::string outputFile) {

	std::ofstream acceptance_rates_file(outputFile.c_str());
	if (!acceptance_rates_file.is_open()) {
		std::cerr << "error opening file " << outputFile.c_str()
				<< "for writing acceptance rates.. Acceptance rates could not be saved!!"
				<< std::endl << std::endl;
		return;
	}

	for (size_t i = 0; i < num_simulations.size(); i++) {
		int num_simulation = num_simulations[i];
		double tolerance = tolerances[i];
		double acceptance_rate = acceptance_rates[i];
		acceptance_rates_file << num_simulation << " " << tolerance << " "
				<< acceptance_rate << std::endl;
	}
	acceptance_rates_file.close();
	std::cout << "acceptance rates wrote into " << outputFile.c_str()
			<< std::endl;
}

std::string InsightOutputWriter::getOutputFile() {
	return _output_file_name;
}
} /* namespace INSIGHTv3 */

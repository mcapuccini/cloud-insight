/*
 * OutputWriter.h
 *
 *  Created on: Oct 10, 2012
 *      Author: jan
 */

#ifndef OUTPUTWRITER_H_
#define OUTPUTWRITER_H_

#include <string>

#include <boost/shared_ptr.hpp>

#include "ParticleSet.h"

namespace INSIGHTv3 {
class InsightOutputWriter {
public:
	InsightOutputWriter(std::string outputFileName);
	virtual ~InsightOutputWriter();

	void writeOutput(const ParticleSet& particle_set, size_t s);
	void writeOutput(const ParticleSet& particle_set, size_t s,
			std::string name_suffix);
	void writeParticles(
			const std::vector<parameters>& particles, std::string outputFile);
	void writeWeights(const std::vector<double>& weights, std::string outputFileWeights);
	void writeAcceptanceRate(std::vector<int>& num_simulations,
			std::vector<double>& tolerances,
			std::vector<double>& acceptance_rates);
	void writeAcceptanceRate(std::vector<int>& num_simulations,
			std::vector<double>& tolerances,
			std::vector<double>& acceptance_rates, std::string name_suffix);
	std::string getOutputFile();

private:
	std::string _output_file_name;
};

typedef boost::shared_ptr<InsightOutputWriter> writer_ptr;
} /* namespace INSIGHTv3 */

#endif /* OUTPUTWRITER_H_ */

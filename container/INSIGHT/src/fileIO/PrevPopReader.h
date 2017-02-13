/*
 * PrevPopReader.h
 *
 *  Created on: Sep 28, 2012
 *      Author: jan
 */

#ifndef PREVPOPREADER_H_
#define PREVPOPREADER_H_

#include <string>

#include "ParticleSet.h"

namespace INSIGHTv3 {

class PrevPopReader {
public:
	PrevPopReader();
	virtual ~PrevPopReader();
	void readPreviousPop(std::string pop_file, int num_params,
			ParticleSet* particle_set);
	void readPreviousPop(std::string pop_file, int num_params,
			ParticleSet* particle_set,
			std::vector<double>* prev_acceptance_rates,
			std::vector<double>* prev_tolerances,
			std::vector<int>* prev_num_simulations);
	void readPrevAcceptanceRates(std::string acceptance_rate_file,
			std::vector<double>* prev_acceptance_rates,
			std::vector<double>* prev_tolerances,
			std::vector<int>* prev_num_simulations);

};

} /* namespace INSIGHTv3 */

#endif /* PREVPOPREADER_H_ */

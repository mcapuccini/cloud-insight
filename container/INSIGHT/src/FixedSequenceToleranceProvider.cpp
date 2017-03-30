/*
 * FixedSequenceToleranceProvider.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: jan
 *
 *      This class provides a sequence of tolerances for the SMC algorithm. This is a particular implementation of the InsightToleranceProvider interface.
 *      It is the simplest version of tolerance provider, since it just provides a fixed sequence.
 */

#include "FixedSequenceToleranceProvider.h"

#include <iostream>

#include "KolmogorovComputer.h"

namespace INSIGHTv3 {

FixedSequenceToleranceProvider::FixedSequenceToleranceProvider(int s,
		MODELS model, KolmogorovComputerPtr kolmogorov_computer) :
		InsightToleranceProvider(s), _kolmogorov_computer(kolmogorov_computer) {

	_initForModel(model);

	_current_simulation_it = _num_simulations.begin();

	while (s > *_current_simulation_it) {
		_current_simulation_it++;
	}
}

FixedSequenceToleranceProvider::~FixedSequenceToleranceProvider() {
// TODO Auto-generated destructor stub
}

int FixedSequenceToleranceProvider::getCurrentSimulationNbr() {
	return *_current_simulation_it;
}

double FixedSequenceToleranceProvider::getCurrentTolerance() {
	double tolerance;
	if (_tolerance_by_s.count(*_current_simulation_it)) {
		tolerance = _tolerance_by_s.at(*_current_simulation_it);
	} else {
		tolerance = _kolmogorov_computer->getThresholdForS(
				*_current_simulation_it);
		_tolerance_by_s.insert(
				std::pair<int, double>(*_current_simulation_it,
						tolerance));
	}
	return tolerance;
}

void FixedSequenceToleranceProvider::next() {
	_current_simulation_it++;
}

void FixedSequenceToleranceProvider::_initForModel(MODELS model) {
	switch (model) {
	case MODEL_BIRTH_DEATH:
		_fillForBirthDeath();
		break;
	case MODEL_BIRTH_DEATH_FL:
		_fillForBirthDeath();
		break;
	case MODLE_LAC_GFP:
		_fillForLacGfp();
		break;
	default:
		break;
	}
}

void FixedSequenceToleranceProvider::_fillForBirthDeath() {
	static const int simulation_nbrs[] = { 8, 30, 100, 300, 500, 750, 1000,
			1500, 2000, 3000, 4000, 5000 };

	_num_simulations = std::vector<int>(simulation_nbrs,
			simulation_nbrs
					+ sizeof(simulation_nbrs) / sizeof(simulation_nbrs[0]));

}

void FixedSequenceToleranceProvider::_fillForLacGfp() {
	static const int simulation_nbrs[] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
			14, 15, 18, 21, 24, 27, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80,
			85, 90, 95, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200,
			210, 220, 230, 240, 250, 275, 300, 325, 350, 375, 400, 425, 450,
			475, 500, 600, 700, 800, 900, 1000 };

	_num_simulations = std::vector<int>(simulation_nbrs,
			simulation_nbrs
					+ sizeof(simulation_nbrs) / sizeof(simulation_nbrs[0]));

}

} /* namespace INSIGHTv3 */

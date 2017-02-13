/*
 * BirthDeathModel.cpp
 *
 *  Created on: Jan 27, 2014
 *      Author: jan
 *
 *      Birth Death model where the data is assumed to be directly observable
 */

#include "BirthDeathModel.h"

#include <stddef.h>
#include <string>

namespace INSIGHTv3 {

BirthDeathModel::BirthDeathModel(RngPtr r, const std::vector<int> output_states) :
		ModelRnd(r, _NUM_SPECIES, _NUM_PARAMETERS, _NUM_REACTIONS,
				"Birth-Death process of a single chemical species (BIRTHDEATH)",
				_MODEL, output_states), _dist_poisson(*_r, PoissonMapping(4)) {

}

BirthDeathModel::~BirthDeathModel() {
}

void BirthDeathModel::_updatePropensities(const state& X,
		const parameters& params, propensities* props) const {

	// Recover species from X vector
	double X1 = X(0);

	// Recover parameters from params vector
	double k1 = params(0);
	double k2 = params(1);

	// Evaluate the propensities
	(*props)(0) = k1;
	(*props)(1) = k2 * X1;
}

void BirthDeathModel::_updateState(const size_t reaction_index,
		EiVectorRef X) const {
	// Update the state vector according to which reaction fired
	switch (reaction_index) {
	case 0:
		X(0) += 1;
		break;
	case 1:
		X(0) -= 1;
		break;
	}
}

void BirthDeathModel::_createOutputState(const state& X,
		const parameters& params, EiVectorRef output) {
	output(0) = X(0);
}

void BirthDeathModel::_setInitialConditions(EiVectorRef X) {
	X(0) = _dist_poisson();
}

} /* namespace INSIGHTv3 */

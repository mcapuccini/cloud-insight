/*
 * BirthDeatFlhModel.cpp
 *
 *  Created on: Jan 24, 2014
 *      Author: jan
 *
 *      Birth Death model where the observed data is assumed to be fluorescent
 */

#include "BirthDeathFlModel.h"

#include <string>

#include "INSIGHTv3.h"

namespace INSIGHTv3 {

BirthDeathFlModel::BirthDeathFlModel(
		const std::vector<EiVector> background_fluorescence, RngPtr r,
		std::vector<int> output_states) :
		ModelFl(background_fluorescence, r, _NUM_SPECIES,
				_NUM_PARAMETERS + 2 * output_states.size(), _NUM_REACTIONS,
				"Birth-Death process of a single chemical species (BIRTHDEATH)",
				_MODEL, output_states), _dist_poisson(*_r, PoissonMapping(4)) {

}

BirthDeathFlModel::~BirthDeathFlModel() {
}

void BirthDeathFlModel::_updatePropensities(const state& X,
		const parameters& params, propensities* props) const {

	// Recover species from X vector
	double X1 = X(0);

	// Recover parameters from params vector
	int begin_index = 2 * output_states.size(); // the first parameters are reserved for the mean and std of the fluourescence
	double k1 = params(begin_index);
	double k2 = params(begin_index + 1);

	// Evaluate the propensities
	(*props)(0) = k1;
	(*props)(1) = k2 * X1;
}

void BirthDeathFlModel::_updateState(const size_t reaction_index,
		EiVectorRef X) const {
	// Update the state vector according to which reaction fired
	switch (reaction_index) {
	case 0:
		X(0) = X(0) + 1;
		break;
	case 1:
		X(0) = X(0) - 1;
		break;
	}
}

void BirthDeathFlModel::_createOutputState(const state& X,
		const parameters& params, EiVectorRef output) {

	EiConstVectorRef mu = params.head(output_states.size());
	EiConstVectorRef std = params.block(output_states.size(), 0,
			output_states.size(), 1);
	_addFlLevel(X, mu, std, output);
}

void BirthDeathFlModel::_setInitialConditions(EiVectorRef X) {
	X(0) = _dist_poisson();
}

}/* namespace INSIGHTv3 */

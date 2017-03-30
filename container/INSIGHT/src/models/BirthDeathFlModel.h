/*
 * BirthDeathFlModel.h
 *
 *  Created on: Jan 24, 2014
 *      Author: jan
 */

#ifndef BIRTHDEATHFLMODEL_H_
#define BIRTHDEATHFLMODEL_H_

#include "ModelFl.h"

#include <stddef.h>

namespace INSIGHTv3 {

class BirthDeathFlModel: public INSIGHTv3::ModelFl {
public:
	BirthDeathFlModel(
			const std::vector<EiVector> background_fluorescence, RngPtr r,
			std::vector<int> output_states);
	virtual ~BirthDeathFlModel();

protected:
	void _updatePropensities(const state& X, const parameters& params,
			propensities* props) const;
	void _updateState(const size_t reaction_index, EiVectorRef X) const;
	void _createOutputState(const state& X, const parameters& params,
			EiVectorRef output);
	void _setInitialConditions(EiVectorRef X);

private:

	static const size_t _NUM_SPECIES = 1;
	static const size_t _NUM_PARAMETERS = 2;
	static const size_t _NUM_REACTIONS = 2;
	static const MODELS _MODEL = MODEL_BIRTH_DEATH_FL;

	DistPoisson _dist_poisson;

};

} /* namespace INSIGHTv3 */

#endif /* BIRTHDEATHFLMODEL_H_ */

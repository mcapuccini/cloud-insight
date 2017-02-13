/*
 * BirthDeathModel.h
 *
 *  Created on: Jan 27, 2014
 *      Author: jan
 */

#ifndef BIRTHDEATHMODEL_H_
#define BIRTHDEATHMODEL_H_

#include "ModelRnd.h"

#include <stddef.h>

namespace INSIGHTv3 {

class BirthDeathModel: public INSIGHTv3::ModelRnd {
public:
	BirthDeathModel(RngPtr r, const std::vector<int> output_states);
	virtual ~BirthDeathModel();

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
	static const MODELS _MODEL = MODEL_BIRTH_DEATH;

	DistPoisson _dist_poisson;

};

} /* namespace INSIGHTv3 */

#endif /* BIRTHDEATHMODEL_H_ */

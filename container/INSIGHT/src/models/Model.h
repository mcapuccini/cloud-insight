/*
 * Model.h
 *
 *  Created on: Jan 24, 2014
 *      Author: jan
 */

#ifndef MODEL_H_
#define MODEL_H_

#include <stddef.h>
#include <string>
#include <vector>

#include "MatrixTypes.h"

namespace INSIGHTv3 {

typedef EiVector state;
typedef EiVector parameters;
typedef EiVector propensities;

enum MODELS {
	MODLE_LAC_GFP = 10, MODEL_BIRTH_DEATH = 20, MODEL_BIRTH_DEATH_FL = 8,
};

class Model {
public:
	Model(const int num_states, const int num_params, const int num_reactions,
			const std::string model_name, const MODELS model,
			const std::vector<int> output_states);
	virtual ~Model();

	void updatePropensities(const state& X, const parameters& params,
			propensities* props) const;
	void updateState(const size_t reaction_index, EiVectorRef X) const;
	void createOutputState(const state& X, const parameters& params,
			EiVectorRef output);
	void setInitialConditions(EiVectorRef X);
	void checkDimensions(const state& X, const parameters& params,
			const propensities& props) const;

	const int num_species;
	const int num_params;
	const int num_reactions;
	const std::string model_name;
	const MODELS model;
	const int num_outputs;
	const std::vector<int> output_states;

private:
	void _checkStateDimension(const state& X) const;
	void _checkParameterDimension(const parameters& params) const;
	void _checkPropDimension(const propensities& props) const;
	void _checkReactionNumber(int reaction_index) const;
	void _checkOutputState(const state& output) const;

protected:
	virtual void _updatePropensities(const state& X, const parameters& params,
			propensities* props) const = 0;
	virtual void _updateState(const size_t reaction_index,
			EiVectorRef X) const = 0;
	virtual void _createOutputState(const state& X, const parameters& params,
			EiVectorRef output) = 0;
	virtual void _setInitialConditions(EiVectorRef X) = 0;
};

} /* namespace INSIGHTv3 */

#endif /* MODEL_H_ */

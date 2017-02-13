/*
 * Model.cpp
 *
 *  Created on: Jan 24, 2014
 *      Author: jan
 *
 *      This is a general implementation of a Stochastic model. All particular models must inherit from this class!
 */

#include "Model.h"

#include <Eigen/src/Core/util/ForwardDeclarations.h>
#include <iostream>

#include "DimensionMissmatchException.h"

namespace INSIGHTv3 {

Model::Model(const int num_states, const int num_params,
		const int num_reactions, const std::string model_name,
		const MODELS model, std::vector<int> output_states) :
		num_species(num_states), num_params(num_params), num_reactions(
				num_reactions), model_name(model_name), model(model), num_outputs(
				output_states.size()), output_states(output_states) {
#ifdef DEBUG
	if(num_outputs > num_species) {
		std::ostringstream os;
		os << "Model has " << num_species
		<< " species but provided output vector has length " << num_outputs
		<< "! ";
		throw DimensionMissmatchException(os.str().c_str());
	}
#endif

}

Model::~Model() {
	// TODO Auto-generated destructor stub
}

void Model::updatePropensities(const state& X, const parameters& params,
		propensities* props) const {

#ifdef DEBUG
	checkDimensions(X, params, *props);
#endif

	_updatePropensities(X, params, props);

}

void Model::updateState(const size_t reaction_index, EiVectorRef X) const {

#ifdef DEBUG
	_checkStateDimension(X);
#endif

	_updateState(reaction_index, X);
}

void Model::createOutputState(const state& X, const parameters& params,
		EiVectorRef output) {
#ifdef DEBUG
	_checkOutputState(output);
#endif

	_createOutputState(X, params, output);
}

void Model::setInitialConditions(EiVectorRef X) {
#ifdef DEBUG
	_checkStateDimension(X);
#endif

	_setInitialConditions(X);
}

void Model::checkDimensions(const state& X, const parameters& params,
		const propensities& props) const {
	std::ostringstream os;
	try {
		_checkStateDimension(X);
	} catch (DimensionMissmatchException& e) {
		os << e.what() << std::endl;
	}

	try {
		_checkParameterDimension(params);
	} catch (DimensionMissmatchException& e) {
		os << e.what() << std::endl;
	}

	try {
		_checkPropDimension(props);
	} catch (DimensionMissmatchException& e) {
		os << e.what() << std::endl;
	}

	if (os.str().size() > 0) {
		throw DimensionMissmatchException(os.str().c_str());
	}

}

void Model::_checkStateDimension(const state& X) const {
	if (X.size() != num_species) {
		std::ostringstream os;
		os << "Model has " << num_species
				<< " states but provided state vector has length " << X.size()
				<< "! ";
		throw DimensionMissmatchException(os.str().c_str());
	}

}

void Model::_checkParameterDimension(const parameters& params) const {
	std::ostringstream os;
	if (params.size() != num_params) {
		os << "Model has " << num_params
				<< " parameters but provided parameter vector has length "
				<< params.size() << "! ";
		throw DimensionMissmatchException(os.str().c_str());
	}
}

void Model::_checkPropDimension(const propensities& props) const {
	if (props.size() != num_reactions) {
		std::ostringstream os;
		os << "Model has " + num_reactions
				<< " reactions but provided propensity vector has length "
				<< props.size() << "! ";
		throw DimensionMissmatchException(os.str().c_str());
	}
}

void Model::_checkReactionNumber(int reaction_index) const {
	if (reaction_index >= num_reactions) {
		std::ostringstream os;
		os << "Model has " + num_reactions
				<< " reactions but reaction index is " << reaction_index
				<< "! ";
		throw DimensionMissmatchException(os.str().c_str());
	}
}

void Model::_checkOutputState(const state& output) const {
	if (output.size() != num_outputs) {
		std::ostringstream os;
		os << "Model has " + num_outputs
				<< " outpus but output vector has size " << output.size()
				<< "! ";
		throw DimensionMissmatchException(os.str().c_str());
	}
}

} /* namespace INSIGHTv3 */

/*
 * ModelFactory.cpp
 *
 *  Created on: Jan 31, 2014
 *      Author: jan
 *
 *      This is a class that creates a model depending on the provided model number. Of course all models can also be created manually.
 */

#include "ModelFactory.h"

#include "IllegalArgumentException.h"
#include "Model.h"
#include "Models.h"
#include "INSIGHTv3.h"

namespace INSIGHTv3 {

ModelFactory::ModelFactory() {
	// TODO Auto-generated constructor stub

}

ModelFactory::~ModelFactory() {
	// TODO Auto-generated destructor stub
}

Model* ModelFactory::getModel(const MODELS model_nbr, RngPtr r, std::vector<int> output_states) {
	switch (model_nbr) {
	case (MODEL_BIRTH_DEATH): {
		return new BirthDeathModel(r, output_states);
	}
	case (MODEL_BIRTH_DEATH_FL): {
		throw IllegalArgumentException("wrong model number...");
	}

	default: {
		throw IllegalArgumentException("wrong model number!");
	}
	}
}

Model* ModelFactory::getModelWithFl(const MODELS model_nbr,RngPtr r, std::vector<int> output_states, std::vector<EiVector> bg_matrix) {
	switch (model_nbr) {
	case (MODEL_BIRTH_DEATH_FL): {
		return new BirthDeathFlModel(bg_matrix, r,  output_states);
	}
	case (MODLE_LAC_GFP): {
		return new LacGfp7Model(bg_matrix, r, output_states);
	}

	default: {
		throw IllegalArgumentException("wrong model number!");
	}
	}
}

bool ModelFactory::modelRequiresBg(const MODELS model_nbr) {
	switch (model_nbr) {
	case (MODEL_BIRTH_DEATH): {
		return false;
	}
	case (MODEL_BIRTH_DEATH_FL): {
		return true;
	}
	case (MODLE_LAC_GFP): {
			return true;
		}
	default: {
		throw IllegalArgumentException("wrong model number!");
	}
	}
}

} /* namespace INSIGHTv3 */

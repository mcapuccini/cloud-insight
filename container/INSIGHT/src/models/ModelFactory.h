/*
 * ModelFactory.h
 *
 *  Created on: Jan 31, 2014
 *      Author: jan
 */

#ifndef MODELFACTORY_H_
#define MODELFACTORY_H_

#include "Model.h"
#include "INSIGHTv3.h"

namespace INSIGHTv3 {

class ModelFactory {
public:
	ModelFactory();
	virtual ~ModelFactory();

	static Model* getModel(const MODELS model_nbr, RngPtr r,
			std::vector<int> output_states);
	static Model* getModelWithFl(const MODELS model_nbr, RngPtr r,
			std::vector<int> output_states, std::vector<EiVector> bg_matrix);
	static bool modelRequiresBg(const MODELS model_nbr);
};

} /* namespace INSIGHTv3 */

#endif /* MODELFACTORY_H_ */

/*
 * ModelRnd.h
 *
 *  Created on: Feb 5, 2014
 *      Author: jan
 *
 *      This is a general model that needs a random number generator.
 */

#ifndef MODELRND_H_
#define MODELRND_H_

#include "Model.h"
#include "INSIGHTv3.h"

namespace INSIGHTv3 {

class ModelRnd: public Model {
public:
	ModelRnd(RngPtr r, const int num_states, const int num_params,
			const int num_reactions, const std::string model_name,
			const MODELS model, const std::vector<int> output_states) :
			Model(num_states, num_params, num_reactions, model_name, model,
					output_states), _r(r) {
	}
	;
	virtual ~ModelRnd() {
	}
	;

protected:
	RngPtr _r;

};

} /* namespace INSIGHTv3 */

#endif /* MODELRND_H_ */

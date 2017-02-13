/*
 * ModelDescription.h
 *
 *  Created on: Feb 3, 2014
 *      Author: jan
 */

#ifndef MODELDESCRIPTION_H_
#define MODELDESCRIPTION_H_

#include <vector>

#include "MatrixTypes.h"
#include "Model.h"

namespace INSIGHTv3 {

class ModelDescription {
public:
	ModelDescription(Model* model,
			const std::vector<EiVector>* original_data,
			const EiVector* lower_bounds, const EiVector* upper_bounds,
			const std::vector<double>* times) :
			model(model), original_data(original_data), lower_bounds_parameter(
					lower_bounds), upper_bounds_parameter(upper_bounds), times(times) {
	}
	;
	virtual ~ModelDescription() {
		delete model;
		delete original_data;
		delete lower_bounds_parameter;
		delete upper_bounds_parameter;
		delete times;
	}
	;

	Model* model;
	const std::vector<EiVector>* original_data;
	const EiVector* lower_bounds_parameter;
	const EiVector* upper_bounds_parameter;
	const std::vector<double>* times;
};

} /* namespace INSIGHTv3 */

#endif /* MODELDESCRIPTION_H_ */

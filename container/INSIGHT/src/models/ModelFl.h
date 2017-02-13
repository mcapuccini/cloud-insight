/*
 * ModelFl.h
 *
 *  Created on: Feb 5, 2014
 *      Author: jan
 */

#ifndef MODELFL_H_
#define MODELFL_H_

#include <ModelRnd.h>

namespace INSIGHTv3 {

class ModelFl: public ModelRnd {
public:
	ModelFl(const std::vector<EiVector> background_fluorescence, RngPtr r,
			const int num_states, const int num_params, const int num_reactions,
			const std::string model_name, const MODELS model,
			const std::vector<int> output_states);
	virtual ~ModelFl();

protected:
	void _addFlLevel(const state& X, EiConstVectorRef mu,
			EiConstVectorRef std, EiVectorRef output);
	const std::vector<EiVector> _background_fluorescence;

	DistUniform _dist_uniform;
	DistNormal _dist_normal;
	bool _use_log_scale;
};

} /* namespace INSIGHTv3 */

#endif /* MODELFL_H_ */

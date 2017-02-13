/*
 * ModelFl.cpp
 *
 *  Created on: Feb 5, 2014
 *      Author: jan
 *
 *      This is a general implementation of a model that assumes that the observed data is fluorescent
 */

#include <cmath>

#include "ModelFl.h"
#include "DimensionMissmatchException.h"

namespace INSIGHTv3 {

ModelFl::ModelFl(const std::vector<EiVector> background_fluorescence, RngPtr r,
		const int num_states, const int num_params, const int num_reactions,
		const std::string model_name, const MODELS model,
		const std::vector<int> output_states) :
		ModelRnd(r, num_states, num_params, num_reactions, model_name, model,
				output_states), _background_fluorescence(
				background_fluorescence), _dist_uniform(*_r), _dist_normal(
				DistNormal(*_r, NormalMapping())), _use_log_scale(false) {

}

ModelFl::~ModelFl() {
}

void ModelFl::_addFlLevel(const state& X, EiConstVectorRef mu,
		EiConstVectorRef sigma, EiVectorRef output) {

#ifdef DEBUG
	if (mu.size() != sigma.size() || mu.size() != output.size()) {
		throw DimensionMissmatchException(
				"Not enough fluorescence parameters for output provided!");
	}
#endif

	for (int i = 0; i < output.size(); i++) {
		double sample_bg = 0.0, sample_fl = 0.0;
		double original_state = X(output_states[i]);

		const EiVector* bg = &(_background_fluorescence[i]);
		int sample_index = floor(_dist_uniform() * bg->size());

		sample_bg = (*bg)(sample_index);

		if (original_state > 0) {
			sample_fl = mu(i) * original_state
					+ _dist_normal()
							* sqrt(sigma(i) * sigma(i) * original_state);
			sample_fl = fmax(0.0, sample_fl);
		}

		if (_use_log_scale) {
			//in this case it is assumed that all provided data is in log scale and the fluourecence needs to be adjusted
			output(i) = log10(pow(10, sample_bg) + sample_fl);
		} else {
			output(i) = sample_bg + sample_fl;
		}
	}
}

}
/* namespace INSIGHTv3 */

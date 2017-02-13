/*
 * ScaledUniformKernel.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: jan
 *
 *      This class is a particular kernel, where the width of each uniform kernel scales with the considered particle
 */

#include "ScaledUniformKernel.h"

#include <cmath>

namespace INSIGHTv3 {

ScaledUniformKernel::ScaledUniformKernel(EiVector lower_bounds,
		EiVector upper_bounds, RngPtr rng, double epsilon) :
		DistributionKernel(lower_bounds, upper_bounds, rng), _dist(*rng), _epsilon(
				epsilon) {
	// TODO Auto-generated constructor stub

}

ScaledUniformKernel::~ScaledUniformKernel() {
	// TODO Auto-generated destructor stub
}

void ScaledUniformKernel::_computeSample(const parameters& perturbed_particle,
		parameters* sampled_particle) {

	for (int i = 0; i < perturbed_particle.size(); i++) {
		double ub = fmin(perturbed_particle(i) * (1 + _epsilon),
				_upper_bounds(i));
		double lb = fmax(perturbed_particle(i) * (1 - _epsilon),
				_lower_bounds(i));
		double rand = _dist();
		(*sampled_particle)(i) = lb + (rand) * (ub - lb);
	}
}

double ScaledUniformKernel::_getLikelihoodForParticle(
		const parameters& perturbed_particle,
		const parameters& sampled_particle) {

	double likelihood = 1.0;
	for (int i = 0; i < perturbed_particle.size(); i++) {
		double ub = fmin(perturbed_particle(i) * (1 + _epsilon),
				_upper_bounds(i));
		double lb = fmax(perturbed_particle(i) * (1 - _epsilon),
				_lower_bounds(i));

		if (sampled_particle(i) >= lb && sampled_particle(i) <= ub) {
			likelihood = likelihood * (1.0 / (ub - lb));
		} else {
			likelihood = 0.0;
			break;
		}
	}
	return likelihood;
}

void ScaledUniformKernel::_updateSeed(int seed) {
	_dist = DistUniform(*_rng);
}
} /* namespace INSIGHTv3 */

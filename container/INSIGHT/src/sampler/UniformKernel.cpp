/*
 * UniformKernel.cpp
 *
 *  Created on: Jan 27, 2014
 *      Author: jan
 *
 *      This class just represents a uniform kernel.
 */

#include "UniformKernel.h"

#include <cstdlib>
#include <cmath>

#include "INSIGHTv3.h"
#include "MatrixTypes.h"

namespace INSIGHTv3 {

UniformKernel::UniformKernel(EiVector lower_bounds, EiVector upper_bounds,
		RngPtr rng, EiVector widths) :
		DistributionKernel(lower_bounds, upper_bounds, rng), _dist(*rng), _widths(
				widths) {

}

UniformKernel::~UniformKernel() {
	// TODO Auto-generated destructor stub
}

void UniformKernel::_computeSample(const parameters& perturbed_particle,
		parameters* sampled_particle) {

	for (int i = 0; i < _widths.size(); i++) {
		double ub = fmin(perturbed_particle(i) + (_widths(i) / 2.0),
				_upper_bounds(i));
		double lb = fmax(perturbed_particle(i) - (_widths(i) / 2.0),
				_lower_bounds(i));
		double rand = _dist();
		(*sampled_particle)(i) = lb + (rand) * (ub - lb);
	}
}

double UniformKernel::_getLikelihoodForParticle(
		const parameters& perturbed_particle,
		const parameters& sampled_particle) {

	double likelihood = 1.0;
	for (int i = 0; i < _widths.size(); i++) {
		double ub = fmin(perturbed_particle(i) + (_widths(i) / 2.0),
				_upper_bounds(i));
		double lb = fmax(perturbed_particle(i) - (_widths(i) / 2.0),
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

void UniformKernel::_updateSeed(int seed) {
	_dist = DistUniform(*_rng);
}
} /* namespace INSIGHTv3 */

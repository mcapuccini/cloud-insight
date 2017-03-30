/*
 * UniformSampler.cpp
 *
 *  Created on: Feb 5, 2014
 *      Author: jan
 *
 *      This class represents a uniform sampler
 */

#include <cmath>

#include "UniformSampler.h"

namespace INSIGHTv3 {

UniformSampler::UniformSampler(RngPtr rng, const EiVector lower_bounds,
		const EiVector upper_bounds) : ParticleSampler(rng, lower_bounds, upper_bounds), _dist(*rng) {
	// TODO Auto-generated constructor stub
}

UniformSampler::~UniformSampler() {
	// TODO Auto-generated destructor stub
}

void UniformSampler::sampleParticle(parameters* params) {
	for (int j = 0; j < params->size(); j++) {
		(*params)(j) = _runif(_lower_bound(j), _upper_bound(j));
	}
}

double UniformSampler::_runif(double a, double b) {
	double rand = _dist();
	return a + (b - a) * rand;
}

double UniformSampler::getLikelihoodForParticle(const parameters& params) {
	double prob = 0.0;
	for (int i = 0; i < params.size(); i++) {
		prob += log(1.0 / (_upper_bound(i) - _lower_bound(i)));
	}

	return exp(prob);
}

void UniformSampler::_updateSeed(int seed){
	_dist = DistUniform(*_rng);
}
} /* namespace INSIGHTv3 */

/*
 * KernelParticleSampler.cpp
 *
 *  Created on: Jan 27, 2014
 *      Author: jan
 *
 *      This class is a general implementation of a sampler that uses Kernel Density Estimation. A suitable Kernel needs to be provided.
 */

#include "KernelParticleSampler.h"

#include <stddef.h>
#include <vector>

#include "INSIGHTv3.h"
#include "MathUtils.h"

namespace INSIGHTv3 {

KernelParticleSampler::KernelParticleSampler(RngPtr rng,
		const EiVector lower_bounds, const EiVector upper_bounds,
		const ParticleSet previous_particles, kernel_ptr kernel,
		bool ignore_weights) :
		ParticleSampler(rng, lower_bounds, upper_bounds), _kernel(kernel), _previous_particles(
				previous_particles), _dist(*rng), _dist_uniform_int(*rng,
				UniformIntMapping(0, previous_particles.getNumberParticles() - 1)), _ignore_weights(
				ignore_weights) {
	// TODO Auto-generated constructor stub

}

KernelParticleSampler::~KernelParticleSampler() {
	// TODO Auto-generated destructor stub
}

void KernelParticleSampler::sampleParticle(parameters* params) {
	const std::vector<parameters>& particles =
			_previous_particles.getParticles();
	bool particle_feasible = false;

	while (!particle_feasible) {
		size_t ix;
		if (!_ignore_weights) {
			ix = MathUtils::uniformSampleNormalizedBinarySearch(
					_previous_particles.getWeightSum(), _dist());
		} else {
			ix = _dist_uniform_int();
		}
		particle_feasible = true;
		_kernel->computeSample(particles[ix], params);

		for (int j = 0; j < params->size(); j++) {
			if (_lower_bound(j) == _upper_bound(j)) {
				(*params)(j) = _lower_bound(j);
			}
			double theta = (*params)(j);
			if ((theta < _lower_bound(j)) || (theta > _upper_bound(j))) {
				particle_feasible = false;
				break;
			}
		}
	}
}

double KernelParticleSampler::getLikelihoodForParticle(
		const parameters& params) {

	double likelihood = 0.0;
	const std::vector<parameters>& particles =
			_previous_particles.getParticles();
	const std::vector<double>& weights = _previous_particles.getWeights();

	for (size_t i = 0; i < _previous_particles.getNumberParticles(); i++) {
		double kernel_likelihood = _kernel->getLikelihoodForParticle(
				particles[i], params);
		if (!_ignore_weights) {
			likelihood += weights[i] * kernel_likelihood;
		} else {
			likelihood += kernel_likelihood;
		}
	}
	return likelihood;
}

void KernelParticleSampler::updatePreviousParticles(
		ParticleSet previous_particles) {
	_previous_particles = previous_particles;
}

void KernelParticleSampler::_updateSeed(int seed) {
	_dist = DistUniform(*_rng);
	_kernel->updateSeed(seed);
}

} /* namespace INSIGHTv3 */

/*
 * KernelSamplerFactory.cpp
 *
 *  Created on: Jan 31, 2014
 *      Author: jan
 *
 *      This class is a factory tha creates KernelParticleSampler. It is an interface that needs to be implemented for each particular Kernel
 */

#include "KernelSamplerFactory.h"

#include <boost/make_shared.hpp>
#include <boost/ref.hpp>

#include "KernelParticleSampler.h"

namespace INSIGHTv3 {

KernelSamplerFactory::KernelSamplerFactory(RngPtr rng,
		int num_parameters, EiVector lower_bounds, EiVector upper_bounds, bool ignore_weights) :
		InsightParticleSamplerFactory(lower_bounds, upper_bounds, ignore_weights), _rng(rng), _num_parameters(
				num_parameters) {

}

KernelSamplerFactory::~KernelSamplerFactory() {
}

sampler_ptr KernelSamplerFactory::createSampler(ParticleSet& prev_particle_set,
		double tolerance, int num_simulations) {

	kernel_ptr kernel = _getKernel(prev_particle_set, tolerance,
			num_simulations);

	return boost::make_shared<KernelParticleSampler>(boost::ref(_rng), _lower_bounds,
			_upper_bounds, prev_particle_set, kernel, _ignore_weights);
}

} /* namespace INSIGHTv3 */

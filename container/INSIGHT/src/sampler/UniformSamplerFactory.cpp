/*
 * UniformSamplerFactory.cpp
 *
 *  Created on: Jan 31, 2014
 *      Author: jan
 *
 *      This is the factory class for the UniformSampler
 */

#include "UniformSamplerFactory.h"

#include <boost/make_shared.hpp>
#include <boost/ref.hpp>

#include "UniformKernel.h"

namespace INSIGHTv3 {

UniformSamplerFactory::UniformSamplerFactory(RngPtr rng,
		int num_parameters, EiVector lower_bounds, EiVector upper_bounds, bool ignore_weights) :
		KernelSamplerFactory(rng, num_parameters, lower_bounds, upper_bounds, ignore_weights) {

}

UniformSamplerFactory::~UniformSamplerFactory() {
}

kernel_ptr UniformSamplerFactory::_getKernel(ParticleSet& prev_particle_set,
		double tolerance, int num_simulations) {
	EiVector widhts(_num_parameters);
	EiVector lower_bounds(_num_parameters);
	EiVector upper_bounds(_num_parameters);

	if (prev_particle_set.getNumberParticles() > 0) {
		prev_particle_set.createBounds(&lower_bounds, &upper_bounds);
	}
	for (int i = 0; i < _num_parameters; i++) {
		widhts[i] = ((upper_bounds[i] - lower_bounds[i])) * tolerance;
	}

	return boost::make_shared<UniformKernel>(_lower_bounds, _upper_bounds, boost::ref(_rng), widhts);
}
} /* namespace INSIGHTv3 */

/*
 * ScaledUniformSamplerFactory.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: jan
 *
 *      This is a factory for the ScaledUniformKernel
 */

#include "ScaledUniformSamplerFactory.h"

#include <boost/make_shared.hpp>
#include <boost/ref.hpp>

#include "ScaledUniformKernel.h"

namespace INSIGHTv3 {

ScaledUniformSamplerFactory::ScaledUniformSamplerFactory(RngPtr rng,
		int num_parameters, EiVector lower_bounds, EiVector upper_bounds, bool ignore_weights) :
		KernelSamplerFactory(rng, num_parameters, lower_bounds, upper_bounds, ignore_weights) {
	// TODO Auto-generated constructor stub

}

ScaledUniformSamplerFactory::~ScaledUniformSamplerFactory() {
	// TODO Auto-generated destructor stub
}

kernel_ptr ScaledUniformSamplerFactory::_getKernel(
		ParticleSet& prev_particle_set, double tolerance,
		int num_simulations) {

	return boost::make_shared<ScaledUniformKernel>( _lower_bounds, _upper_bounds, boost::ref(_rng), tolerance);
}
} /* namespace INSIGHTv3 */

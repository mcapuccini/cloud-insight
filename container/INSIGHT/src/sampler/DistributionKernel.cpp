/*
 * DistributionKernel.cpp
 *
 *  Created on: Jan 27, 2014
 *      Author: jan
 *
 *      This class is an interface for general Kernels for Kernle Density Estimation. It is supposed to be used with KernelParticleSampler
 */

#include "DistributionKernel.h"

#include <iostream>
#include <string>

#include "DimensionMissmatchException.h"
#include "INSIGHTv3.h"

namespace INSIGHTv3 {

DistributionKernel::DistributionKernel( EiVector lower_bounds, EiVector upper_bounds, RngPtr rng) :
		_lower_bounds(lower_bounds), _upper_bounds(upper_bounds), _rng(rng) {
}

DistributionKernel::~DistributionKernel() {
}

void DistributionKernel::computeSample(const parameters& disturbed_particle,
		parameters* sampled_particle) {

#ifdef Debug
		_checkParticleDimensions(disturbed_particle, *sampled_particle);
#endif

	_computeSample(disturbed_particle, sampled_particle);
}

double DistributionKernel::getLikelihoodForParticle(
		const parameters& disturbed_particle,
		const parameters& sampled_particle) {

#ifdef Debug
		_checkParticleDimensions(disturbed_particle, *sampled_particle);
#endif

	return _getLikelihoodForParticle(disturbed_particle, sampled_particle);
}

void DistributionKernel::_checkParticleDimensions(
		const parameters& disturbed_particle,
		const parameters& sampled_particle) const {
	std::ostringstream os;
	if (disturbed_particle.size() != sampled_particle.size()) {
		os << "The perturbed particle has dimension "
				<< disturbed_particle.size()
				<< " and the sample particle has dimension "
				<< sampled_particle.size() << "! ";
		throw DimensionMissmatchException(os.str().c_str());
	}

}
} /* namespace INSIGHTv3 */

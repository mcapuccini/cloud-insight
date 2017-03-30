/*
 * KernelSamplerFactory.h
 *
 *  Created on: Jan 30, 2014
 *      Author: jan
 */

#ifndef KERNELSAMPLERFACTORY_H_
#define KERNELSAMPLERFACTORY_H_

#include "DistributionKernel.h"
#include "InsightParticleSamplerFactory.h"
#include "INSIGHTv3.h"
#include "ParticleSet.h"

namespace INSIGHTv3 {

class KernelSamplerFactory: public INSIGHTv3::InsightParticleSamplerFactory {
public:
	KernelSamplerFactory(RngPtr rng, int num_parameters,
			EiVector lower_bounds, EiVector upper_bounds, bool ignore_weights);
	virtual ~KernelSamplerFactory();

	virtual sampler_ptr createSampler(ParticleSet& prev_particle_set,
			double tolerance, int num_simulations);

protected:
	RngPtr _rng;
	int _num_parameters;

	virtual kernel_ptr _getKernel(ParticleSet& prev_particle_set,
			double tolerance, int num_simulations) = 0;

};

} /* namespace INSIGHTv3 */

#endif /* KERNELSAMPLERFACTORY_H_ */

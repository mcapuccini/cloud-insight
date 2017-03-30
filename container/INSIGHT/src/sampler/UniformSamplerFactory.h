/*
 * UniformSamplerFactory.h
 *
 *  Created on: Jan 31, 2014
 *      Author: jan
 */

#ifndef UNIFORMSAMPLERFACTORY_H_
#define UNIFORMSAMPLERFACTORY_H_

#include <KernelSamplerFactory.h>

namespace INSIGHTv3 {

class UniformSamplerFactory: public INSIGHTv3::KernelSamplerFactory {
public:
	UniformSamplerFactory(RngPtr rng, int num_parameters,
			EiVector lower_bounds, EiVector upper_bounds, bool ignore_weights);
	virtual ~UniformSamplerFactory();

protected:
	kernel_ptr _getKernel(ParticleSet& prev_particle_set, double tolerance,
			int num_simulations);
};

} /* namespace INSIGHTv3 */

#endif /* UNIFORMSAMPLERFACTORY_H_ */

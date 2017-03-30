/*
 * ScaledUniformSamplerFactory.h
 *
 *  Created on: Mar 11, 2014
 *      Author: jan
 */

#ifndef SCALEDUNIFORMSAMPLERFACTORY_H_
#define SCALEDUNIFORMSAMPLERFACTORY_H_

#include "KernelSamplerFactory.h"

namespace INSIGHTv3 {

class ScaledUniformSamplerFactory: public INSIGHTv3::KernelSamplerFactory {
public:
	ScaledUniformSamplerFactory(RngPtr rng, int num_parameters,
					EiVector lower_bounds, EiVector upper_bounds, bool ignore_weights);
	virtual ~ScaledUniformSamplerFactory();


protected:
	kernel_ptr _getKernel(ParticleSet& prev_particle_set, double tolerance,
			int num_simulations);
};

} /* namespace INSIGHTv3 */

#endif /* SCALEDUNIFORMSAMPLERFACTORY_H_ */

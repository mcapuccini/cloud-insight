/*
 * InsightParticleSamplerFactory.h
 *
 *  Created on: Jan 30, 2014
 *      Author: jan
 */

#ifndef INSIGHTPARTICLESAMPLERFACTORY_H_
#define INSIGHTPARTICLESAMPLERFACTORY_H_

#include <boost/smart_ptr/shared_ptr.hpp>

#include "ParticleSampler.h"
#include "DistributionKernel.h"
#include "ParticleSet.h"

namespace INSIGHTv3 {

class InsightParticleSamplerFactory {
public:
	InsightParticleSamplerFactory(EiVector lower_bounds, EiVector upper_bounds, bool ignore_weights) :
			_lower_bounds(lower_bounds), _upper_bounds(upper_bounds), _ignore_weights(ignore_weights) {
	}
	;
	virtual ~InsightParticleSamplerFactory() {
	}
	;

	virtual sampler_ptr createSampler(ParticleSet& prev_particle_set,
			double tolerance, int num_simulations) = 0;

protected:
	EiVector _lower_bounds;
	EiVector _upper_bounds;
	bool _ignore_weights;
};

typedef boost::shared_ptr<InsightParticleSamplerFactory> sampler_factory_ptr;
} /* namespace INSIGHTv3 */

#endif /* INSIGHTPARTICLESAMPLERFACTORY_H_ */

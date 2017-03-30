/*
 * ParticleSampler.h
 *
 *  Created on: Jan 27, 2014
 *      Author: jan
 */

#ifndef PARTICLESAMPLER_H_
#define PARTICLESAMPLER_H_

#include <Eigen/Dense>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/access.hpp>

#include "Model.h"
#include "SSASimulator.h"
#include "INSIGHTv3.h"

namespace INSIGHTv3 {

class ParticleSampler {
public:
	ParticleSampler(RngPtr rng, const EiVector lower_bounds,
			const EiVector upper_bounds) :
			_rng(rng), _lower_bound(lower_bounds), _upper_bound(upper_bounds) {
	}
	;

	virtual ~ParticleSampler() {
	}
	;

	virtual void sampleParticle(parameters* params) = 0;
	virtual double getLikelihoodForParticle(const parameters& params) = 0;
	void setSeed(int seed) {
		_rng->seed(seed);
		_updateSeed(seed);
	}

	friend class ::boost::serialization::access;
protected:

	RngPtr _rng;
	EiVector _lower_bound;
	EiVector _upper_bound;

	virtual void _updateSeed(int seed) = 0;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
	}
};

typedef boost::shared_ptr<ParticleSampler> sampler_ptr;
} /* namespace INSIGHTv3 */

#endif /* PARTICLESAMPLER_H_ */

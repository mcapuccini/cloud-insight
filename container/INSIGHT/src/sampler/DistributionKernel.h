/*
 * DistributionKernel.h
 *
 *  Created on: Jan 27, 2014
 *      Author: jan
 */

#ifndef DISTRIBUTIONKERNEL_H_
#define DISTRIBUTIONKERNEL_H_

#include <boost/serialization/access.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>

#include "INSIGHTv3.h"
#include "Model.h"

namespace INSIGHTv3 {

class DistributionKernel {
public:
	DistributionKernel(EiVector lower_bounds, EiVector upper_bounds, RngPtr rng);
	virtual ~DistributionKernel();

	void computeSample(const parameters& disturbed_particle,
			parameters* sampled_particle);
	double getLikelihoodForParticle(const parameters& disturbed_particle,
			const parameters& sampled_particle);
	void updateSeed(int seed){
		_rng->seed(seed);
		_updateSeed(seed);
	};

	friend class boost::serialization::access;
protected:
	EiVector _lower_bounds;
	EiVector _upper_bounds;
	RngPtr _rng;

	virtual void _computeSample(const parameters& disturbed_particle,
			parameters* sampled_particle) = 0;
	virtual double _getLikelihoodForParticle(
			const parameters& disturbed_particle,
			const parameters& sampled_particle) = 0;

	void _checkParticleDimensions(const parameters& disturbed_particle,
			const parameters& sampled_particle) const;
	virtual void _updateSeed(int seed) = 0;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
	}

}
;

typedef boost::shared_ptr<DistributionKernel> kernel_ptr;

} /* namespace INSIGHTv3 */

#endif /* DISTRIBUTIONKERNEL_H_ */


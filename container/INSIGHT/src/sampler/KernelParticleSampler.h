/*
 * KernelParticleSampler.h
 *
 *  Created on: Jan 27, 2014
 *      Author: jan
 */

#ifndef KERNELPARTICLESAMPLER_H_
#define KERNELPARTICLESAMPLER_H_

#include "ParticleSampler.h"

#include <boost/make_shared.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#include "INSIGHTv3.h"
#include "ParticleSet.h"
#include "DistributionKernel.h"

namespace INSIGHTv3 {
class KernelParticleSampler;
}

namespace boost {
namespace serialization {
template<class Archive>
inline void save_construct_data(Archive & ar,
		const INSIGHTv3::KernelParticleSampler * t,
		const unsigned int file_version);
}
}

namespace INSIGHTv3 {

class KernelParticleSampler: public INSIGHTv3::ParticleSampler {
public:
	KernelParticleSampler(RngPtr rng, const EiVector lower_bounds,
			const EiVector upper_bounds, const ParticleSet previous_particles,
			kernel_ptr kernel, bool ignore_weights);
	virtual ~KernelParticleSampler();

	virtual void sampleParticle(parameters* params);
	virtual double getLikelihoodForParticle(const parameters& params);
	virtual void updatePreviousParticles(ParticleSet previous_particles);

	friend class ::boost::serialization::access;
	template<class Archive> friend void ::boost::serialization::save_construct_data(
			Archive & ar, const KernelParticleSampler * t,
			const unsigned int file_version);

	template<class Archive>
	void serialize(Archive &ar, const unsigned int file_version) {
		ar & boost::serialization::base_object<ParticleSampler>(*this);
	}

protected:
	void _updateSeed(int seed);
private:
	kernel_ptr _kernel;
	ParticleSet _previous_particles;
	DistUniform _dist;
	DistUniformInt _dist_uniform_int;

	bool _ignore_weights;
};

} /* namespace INSIGHTv3 */

namespace boost {
namespace serialization {
template<class Archive>
inline void save_construct_data(Archive & ar,
		const INSIGHTv3::KernelParticleSampler * t,
		const unsigned int file_version) {
	int num_parameters = (t->_lower_bound).size();
	ar << num_parameters;
	for (int i = 0; i < t->_lower_bound.size(); i++) {
		ar << t->_lower_bound(i);
		ar << t->_upper_bound(i);
	}
	ar << t->_previous_particles;
	INSIGHTv3::DistributionKernel* kernel_ptr = t->_kernel.get();
	ar << kernel_ptr;
	bool ignore_weights = t->_ignore_weights;
	ar << ignore_weights;
}

template<class Archive>
inline void load_construct_data(Archive & ar,
		INSIGHTv3::KernelParticleSampler * t, const unsigned int file_version) {
	size_t num_parameters;
	ar >> num_parameters;
	INSIGHTv3::EiVector lower_bounds(num_parameters);
	INSIGHTv3::EiVector upper_bounds(num_parameters);
	for (size_t i = 0; i < num_parameters; i++) {
		ar >> lower_bounds(i);
		ar >> upper_bounds(i);
	}
	INSIGHTv3::ParticleSet particle_set;
	ar >> particle_set;
	INSIGHTv3::DistributionKernel* kernel;
	ar >> kernel;

	bool ignore_weights;
	ar >> ignore_weights;

	shared_ptr<mt19937> rng = make_shared<mt19937>();
	// invoke inplace constructor to initialize instance of my_class
	::new (t) INSIGHTv3::KernelParticleSampler(rng, lower_bounds, upper_bounds,
			particle_set, shared_ptr<INSIGHTv3::DistributionKernel>(kernel), ignore_weights);
}
}
}

#endif /* KERNELPARTICLESAMPLER_H_ */

/*
 * UniformSampler.h
 *
 *  Created on: Feb 5, 2014
 *      Author: jan
 */

#ifndef UNIFORMSAMPLER_H_
#define UNIFORMSAMPLER_H_

#include <boost/smart_ptr/make_shared.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>

#include "ParticleSampler.h"
#include "INSIGHTv3.h"
#include "stddef.h"
#include "iostream"


namespace INSIGHTv3{
class UniformSampler;
}

namespace boost {
namespace serialization {
template<class Archive>
inline void save_construct_data(Archive & ar, const INSIGHTv3::UniformSampler * t,
		const unsigned int file_version);
}
}

namespace INSIGHTv3 {

class UniformSampler: public INSIGHTv3::ParticleSampler {
public:
	UniformSampler(RngPtr rng, const EiVector lower_bounds,
			const EiVector upper_bounds);
	virtual ~UniformSampler();

	void sampleParticle(parameters* params);
	double getLikelihoodForParticle(const parameters& params);

	friend class ::boost::serialization::access;
	template<class Archive> friend void ::boost::serialization::save_construct_data(
			Archive & ar, const UniformSampler * t,
			const unsigned int file_version);

	template<class Archive>
		void serialize(Archive &ar, const unsigned int file_version) {
			ar & boost::serialization::base_object<ParticleSampler>(*this);
		}

protected:
	void _updateSeed(int seed);
private:
	double _runif(double a, double b);
	DistUniform _dist;
};

} /* namespace INSIGHTv3 */

namespace boost{
namespace serialization {
template<class Archive>
inline void save_construct_data(Archive & ar, const INSIGHTv3::UniformSampler * t,
		const unsigned int file_version) {
	int num_parameters = (t->_upper_bound).size();
	ar << num_parameters;
	for (int i = 0; i < t->_upper_bound.size(); i++) {
		ar << t->_lower_bound(i);
		ar << t->_upper_bound(i);
	}
}

template<class Archive>
inline void load_construct_data(Archive & ar, INSIGHTv3::UniformSampler * t,
		const unsigned int file_version) {
	size_t num_parameters;
	ar >> num_parameters;
	INSIGHTv3::EiVector lower_bound(num_parameters);
	INSIGHTv3::EiVector upper_bound(num_parameters);
	for (size_t i = 0; i < num_parameters; i++) {
		ar >> lower_bound(i);
		ar >> upper_bound(i);

	}
	shared_ptr<mt19937> rng_ptr = make_shared<mt19937>();
	// invoke inplace constructor to initialize instance of my_class
	::new (t) INSIGHTv3::UniformSampler(rng_ptr, lower_bound, upper_bound);
}
}
}

#endif /* UNIFORMSAMPLER_H_ */

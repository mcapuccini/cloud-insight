/*
 * ScaledUniformKernel.h
 *
 *  Created on: Mar 11, 2014
 *      Author: jan
 */

#ifndef SCALEDUNIFORMKERNEL_H_
#define SCALEDUNIFORMKERNEL_H_

#include <boost/make_shared.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>

#include "DistributionKernel.h"
#include "Model.h"

namespace INSIGHTv3 {
class ScaledUniformKernel;
}

namespace boost {
namespace serialization {
template<class Archive>
inline void save_construct_data(Archive & ar,
		const INSIGHTv3::ScaledUniformKernel * t,
		const unsigned int file_version);
}
}

namespace INSIGHTv3 {

class ScaledUniformKernel: public DistributionKernel {
public:
	ScaledUniformKernel(
			EiVector lower_bounds, EiVector upper_bounds, RngPtr rng, double _epsilon);
	virtual ~ScaledUniformKernel();

	friend class ::boost::serialization::access;
	template<class Archive> friend void ::boost::serialization::save_construct_data(
			Archive & ar, const ScaledUniformKernel * t,
			const unsigned int file_version);

	template<class Archive>
	void serialize(Archive &ar, const unsigned int file_version) {
		ar & boost::serialization::base_object<DistributionKernel>(*this);
	}
protected:
	void _computeSample(const parameters& disturbed_particle,
			parameters* sampled_particle);
	double _getLikelihoodForParticle(const parameters& disturbed_particle,
			const parameters& sampled_particle);
	void _updateSeed(int seed);

private:
	DistUniform _dist;
	double _epsilon;
};
} /* namespace INSIGHTv3 */

namespace boost {
namespace serialization {
template<class Archive>
inline void save_construct_data(Archive & ar,
		const INSIGHTv3::ScaledUniformKernel * t,
		const unsigned int file_version) {
	double epsilon = t->_epsilon;
	ar << epsilon;

	int num_parameters = t->_lower_bounds.size();
	ar << num_parameters;
	for (int i = 0; i < num_parameters; i++) {
		ar << t->_lower_bounds(i);
		ar << t->_upper_bounds(i);
	}
}

template<class Archive>
inline void load_construct_data(Archive & ar,
		INSIGHTv3::ScaledUniformKernel * t, const unsigned int file_version) {
	double epsilon;
	ar >> epsilon;

	double num_parameters;
	ar >> num_parameters;

	INSIGHTv3::EiVector lower_bounds(num_parameters);
	INSIGHTv3::EiVector upper_bounds(num_parameters);
	for (int i = 0; i < num_parameters; i++) {
		ar >> lower_bounds(i);
		ar >> upper_bounds(i);
	}

	shared_ptr<mt19937> rng = make_shared<mt19937>();
	// invoke inplace constructor to initialize instance of my_class
	::new (t) INSIGHTv3::ScaledUniformKernel(lower_bounds, upper_bounds, rng, epsilon);
}

}
}

#endif /* SCALEDUNIFORMKERNEL_H_ */

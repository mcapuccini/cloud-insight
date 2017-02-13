/*
 * UniformKernel.h
 *
 *  Created on: Jan 27, 2014
 *      Author: jan
 */

#ifndef UNIFORMKERNEL_H_
#define UNIFORMKERNEL_H_

#include <boost/make_shared.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>

#include "DistributionKernel.h"
#include "Model.h"

namespace INSIGHTv3{
class UniformKernel;
}

namespace boost {
namespace serialization {
template<class Archive>
inline void save_construct_data(Archive & ar, const INSIGHTv3::UniformKernel * t,
		const unsigned int file_version);
}
}

namespace INSIGHTv3 {

class UniformKernel: public INSIGHTv3::DistributionKernel {
public:
	UniformKernel(EiVector lower_bounds, EiVector upper_bounds, RngPtr rng, EiVector widths);
	virtual ~UniformKernel();

	friend class ::boost::serialization::access;
		template<class Archive> friend void ::boost::serialization::save_construct_data(
				Archive & ar, const UniformKernel * t,
				const unsigned int file_version);

		template<class Archive>
				void serialize(Archive &ar, const unsigned int file_version) {
					ar & boost::serialization::base_object<DistributionKernel>(*this);
				}
protected:
	void _computeSample(const parameters& perturbed_particle,
			parameters* sampled_particle);
	double _getLikelihoodForParticle(const parameters& perturbed_particle,
			const parameters& sampled_particle);
	void _updateSeed(int seed);

private:
	DistUniform _dist;
	EiVector _widths;
};
} /* namespace INSIGHTv3 */

namespace boost {
namespace serialization {
template<class Archive>
inline void save_construct_data(Archive & ar, const INSIGHTv3::UniformKernel * t,
		const unsigned int file_version) {
	int num_parameters = (t->_widths).size();
	ar << num_parameters;
	for (int i = 0; i < num_parameters; i++) {
		ar << t->_widths(i);
		double lb = t->_lower_bounds(i);
		double ub = t->_upper_bounds(i);
		ar << lb;
		ar << ub;
	}
}

template<class Archive>
inline void load_construct_data(Archive & ar, INSIGHTv3::UniformKernel * t,
		const unsigned int file_version) {
	size_t num_parameters;
	ar >> num_parameters;
	INSIGHTv3::EiVector widths(num_parameters);
	INSIGHTv3::EiVector lower_bounds(num_parameters);
		INSIGHTv3::EiVector upper_bounds(num_parameters);
	for (size_t i = 0; i < num_parameters; i++) {
		ar >> widths(i);
		ar >> lower_bounds(i);
		ar>> upper_bounds(i);
	}
	shared_ptr<mt19937> rng = make_shared<mt19937>();
	// invoke inplace constructor to initialize instance of my_class
	::new (t) INSIGHTv3::UniformKernel(lower_bounds, upper_bounds, rng, widths);
}
}
}

#endif /* UNIFORMKERNEL_H_ */

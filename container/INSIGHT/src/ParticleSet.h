/*
 * ParticleSet.h
 *
 *  Created on: Jan 27, 2014
 *      Author: jan
 */

#ifndef PARTICLESET_H_
#define PARTICLESET_H_

#include <stddef.h>
#include <vector>

#include <boost/serialization/split_member.hpp>

#include "Model.h"

namespace INSIGHTv3 {

class ParticleSet {
public:
	ParticleSet();
	ParticleSet(int num_particles);
	virtual ~ParticleSet();

	const std::vector<parameters>& getParticles() const;
	const std::vector<double>& getWeights() const;
	const std::vector<double>& getWeightSum() const;
	void acceptParticle(parameters accepted_particle, double weight);
	void acceptParticle(double* accepted_particle, double weight,
			int num_parameters);
	size_t getNumberParticles() const;
	void normalizeWeights();
	void createBounds(EiVector* lower_bounds, EiVector* upper_bounds);

	friend class boost::serialization::access;
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const {
		int num_particles = _particles.size();
		ar << num_particles;
		if (num_particles > 0) {
			int num_parameters = _particles[0].size();
			ar << num_parameters;
			for (int i = 0; i < num_particles; i++) {
				parameters vec = _particles[i];
				for (int j = 0; j < vec.size(); j++) {
					double val = vec(j);
					ar << val;
				}
				ar << _weights[i];
				ar << _weight_sum[i];
			}
		}
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version) {
		int num_particles = 0;
		ar >> num_particles;
		if (num_particles > 0) {
			int num_parameters = 0;
			ar >> num_parameters;
			parameters vec(num_parameters);
			for (int i = 0; i < num_particles; i++) {
				for (int j = 0; j < num_parameters; j++) {
					double val = 0;
					ar & val;
					vec(j) = val;
				}
				double weight = 0;
				ar >> weight;
				double weight_sum = 0;
				ar >> weight_sum;

				_particles.push_back(vec);
				_weights.push_back(weight);
				_weight_sum.push_back(weight_sum);
			}
		}
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER();
private:
	std::vector<parameters> _particles;
	std::vector<double> _weights;
	std::vector<double> _weight_sum;
};

} /* namespace INSIGHTv3 */

#endif /* PARTICLESET_H_ */

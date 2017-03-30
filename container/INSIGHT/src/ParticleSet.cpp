/*
 * ParticleSet.cpp
 *
 *  Created on: Jan 27, 2014
 *      Author: jan
 *
 *      This class manages a set of particles with the corresponding weights
 */

#include "ParticleSet.h"

#include <iostream>

#include "DimensionMissmatchException.h"

namespace INSIGHTv3 {

ParticleSet::ParticleSet() {
	// TODO Auto-generated constructor stub
}

ParticleSet::ParticleSet(int num_particles) {
	_particles.reserve(num_particles);
	_weights.reserve(num_particles);
	_weight_sum.reserve(num_particles);
}

ParticleSet::~ParticleSet() {
}

const std::vector<parameters>& ParticleSet::getParticles() const {
	return _particles;
}

const std::vector<double>& ParticleSet::getWeights() const { // TODO Auto-generated destructor stub
	return _weights;
}

const std::vector<double>& ParticleSet::getWeightSum() const {
	return _weight_sum;
}

size_t ParticleSet::getNumberParticles() const {
	return _particles.size();
}

void ParticleSet::acceptParticle(parameters accepted_particle, double weight) {
	_particles.push_back(accepted_particle);
	_weights.push_back(weight);
	_weight_sum.empty() ?
			_weight_sum.push_back(_weights.back()) :
			_weight_sum.push_back(_weight_sum.back() + _weights.back());

}

void ParticleSet::acceptParticle(double* accepted_particle, double weight,
		int num_parameters) {
	EiVector particle = Eigen::Map<EiVector>(accepted_particle, num_parameters);

	_particles.push_back(particle);
	_weights.push_back(weight);
	_weight_sum.empty() ?
			_weight_sum.push_back(_weights.back()) :
			_weight_sum.push_back(_weight_sum.back() + _weights.back());

}

void ParticleSet::normalizeWeights() {
	double sum_weights = _weight_sum.back();
	for (size_t i = 0; i < _weights.size(); i++) {
		_weights[i] = _weights[i] / sum_weights;
		_weight_sum[i] = _weight_sum[i] / sum_weights;
	}
}

void ParticleSet::createBounds(EiVector* lower_bounds, EiVector* upper_bounds) {

#ifdef DEBUG
	if (_particles.size() == 0 || lower_bounds->size() != _particles[0].size() || upper_bounds->size() != _particles[0].size()) {
		std::ostringstream os;
		os << "Cannot compute bounds for particles! Number particles in set: " << _particles.size()
		<< ", particle dimension: " << _particles[0].size() << ", lower bounds number: " << lower_bounds->size()
		<< ", upper bounds number: " << upper_bounds->size();
		throw DimensionMissmatchException(os.str().c_str());
	}
#endif

	size_t number_parameters = lower_bounds->size();

	for (size_t j = 0; j < _particles.size(); j++) {
		const EiVector particle = _particles[j];

		for (size_t i = 0; i < number_parameters; i++) {
			if (j == 0 || particle(i) < (*lower_bounds)(i)) {
				(*lower_bounds)(i) = particle(i);
			}
			if (j == 0 || particle(i) > (*upper_bounds)(i)) {
				(*upper_bounds)(i) = particle(i);
			}
		}

	}
}
} /* namespace INSIGHTv3 */

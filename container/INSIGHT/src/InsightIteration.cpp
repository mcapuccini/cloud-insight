/*
 * InsightIteration.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: jan
 *
 *      This class represents one iteration of the INSIGHT algorithm. It samples particles, simulates the system and accepts the particle if the simulations results
 *      are similar to the observed original data. It does that until it has sufficient particles.
 */

#include <InsightIteration.h>
#include "INSIGHTv3.h"
#include "Model.h"

namespace INSIGHTv3 {

InsightIteration::InsightIteration(sampler_ptr prior_sampler) :
		_prior_sampler(prior_sampler) {
	// TODO Auto-generated constructor stub

}

InsightIteration::~InsightIteration() {
	// TODO Auto-generated destructor stub
}

void InsightIteration::doIteration(int num_particles, int num_parameters,
		sampler_ptr sampler, evaluator_ptr evaluator, ParticleSet* particle_set,
		IterationLogger* logger) {
	time_t tic = clock();

	int num_accepted_particles = 0;
	int num_sampled_particles = 0;
	parameters particle(num_parameters);

	while (num_accepted_particles < num_particles) {
		bool particle_accepted = false;
		while (!particle_accepted) {
			sampler->sampleParticle(&particle);
			num_sampled_particles++;
			particle_accepted = evaluator->isParticleAcceptable(particle);
		}
		double weight = _prior_sampler->getLikelihoodForParticle(particle)
				/ sampler->getLikelihoodForParticle(particle);

		particle_set->acceptParticle(particle, weight);
		num_accepted_particles++;
		logger->print(num_sampled_particles, num_accepted_particles, weight,
				tic);
	}

}

} /* namespace INSIGHTv3 */

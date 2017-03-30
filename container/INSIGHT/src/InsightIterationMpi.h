/*
 * InsightIterationMpi.h
 *
 *  Created on: Feb 11, 2014
 *      Author: jan
 */

#ifndef INSIGHTITERATIONMPI_H_
#define INSIGHTITERATIONMPI_H_

#include "InsightIteration.h"
#include "MpiTags.h"

namespace INSIGHTv3 {

class InsightIterationMpi: public INSIGHTv3::InsightIteration {
public:
	InsightIterationMpi(int num_tasks, sampler_ptr prior_sampler);
	virtual ~InsightIterationMpi();

	void doIteration(int num_particles, int num_parameters, sampler_ptr sampler,
			evaluator_ptr evaluator, ParticleSet* particle_set,
			IterationLogger* logger);

private:
	void _requestNextParticle(size_t process);
	void _receiveParticle(MPI::Status& status, ParticleSet* particle_set);
	void _updateParticleSampler(sampler_ptr sampler);
	void _updateParticleEvaluator(evaluator_ptr evaluator);

	int _num_tasks;
	int _num_new_sampled_particles;
	double _weight;
	double* _particle;
	int _num_parameters;

};

} /* namespace INSIGHTv3 */

#endif /* INSIGHTITERATIONMPI_H_ */

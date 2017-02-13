/*
 * InsightMpiWorker.h
 *
 *  Created on: Feb 12, 2014
 *      Author: jan
 */

#ifndef INSIGHTMPIWORKER_H_
#define INSIGHTMPIWORKER_H_

#include "MpiTags.h"
#include "ParticleSampler.h"
#include "ParticleEvaluatorInsight.h"
#include "INSIGHTv3.h"

namespace INSIGHTv3 {

class InsightMpiWorker {
public:
	InsightMpiWorker(size_t my_rank, size_t num_parameters,
			evaluator_ptr evaluator, sampler_ptr prior_sampler);
	virtual ~InsightMpiWorker();

	virtual void run();
protected:
	const size_t _my_rank;
	const size_t _num_parameters;
	ParticleSampler* _sampler;
	evaluator_ptr _evaluator;

	double* _particle;
	parameters _particle_ei_vector;
	sampler_ptr _prior_sampler;

	void _updateSampler();
	void _updateEvaluator();
	void _provideParticle();
};

} /* namespace INSIGHTv3 */

#endif /* INSIGHTMPIWORKER_H_ */

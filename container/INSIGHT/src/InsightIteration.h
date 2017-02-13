/*
 * InsightIteration.h
 *
 *  Created on: Jan 30, 2014
 *      Author: jan
 */

#ifndef INSIGHTITERATION_H_
#define INSIGHTITERATION_H_

#include "ParticleEvaluatorInsight.h"
#include "ParticleSampler.h"
#include "ParticleSet.h"
#include "IterationLogger.h"

namespace INSIGHTv3 {

class InsightIteration {
public:
	InsightIteration(sampler_ptr prior_sampler);
	virtual ~InsightIteration();

	virtual void doIteration(int num_particles, int num_parameters, sampler_ptr sampler, evaluator_ptr evaluator,
			ParticleSet* particle_set, IterationLogger* logger);

private:
	sampler_ptr _prior_sampler;

};

} /* namespace INSIGHTv3 */

#endif /* INSIGHTITERATION_H_ */

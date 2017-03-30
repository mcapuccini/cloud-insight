/*
 * ParticleEvaluator.h
 *
 *  Created on: Jan 30, 2014
 *      Author: jan
 */

#ifndef PARTICLEEVALUATOR_H_
#define PARTICLEEVALUATOR_H_

#include "Model.h"

namespace INSIGHTv3 {

class ParticleEvaluator {
public:
	ParticleEvaluator() {
	}
	;
	virtual ~ParticleEvaluator() {
	}
	;

	virtual bool isParticleAcceptable(const parameters& particle) = 0;
};

} /* namespace INSIGHTv3 */

#endif /* PARTICLEEVALUATOR_H_ */

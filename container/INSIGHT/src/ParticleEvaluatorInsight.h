/*
 * ParticleEvaluator.h
 *
 *  Created on: Jan 27, 2014
 *      Author: jan
 */

#ifndef PARTICLEEVALUATORINSIGHT_H_
#define PARTICLEEVALUATORINSIGHT_H_

#include "ParticleEvaluator.h"

#include <MatrixTypes.h>
#include <Model.h>
#include <SSASimulator.h>
#include <set>
#include <vector>

#include "INSIGHTv3.h"
#include "KolmogorovComputer.h"

namespace INSIGHTv3 {

class ParticleEvaluatorInsight: public INSIGHTv3::ParticleEvaluator {
public:
	ParticleEvaluatorInsight(Model& model, const std::vector<double> times,
			RngPtr rng, KolmogorovComputerPtr kolmogorov_computer);
	virtual ~ParticleEvaluatorInsight();

	bool isParticleAcceptable(const parameters& particle);
	void setEvaluationParams(int num_simulations, double tolerance);

	int getNumSimulations() {
		return _current_num_simulations;
	}
	;

	double getTolerance() {
		return _current_tolerance;
	}
	;

private:
	Model& _model;
	const std::vector<double> _times;

	int _current_num_simulations;
	double _current_tolerance;
	EiVector _times_of_last_fired_reaction;
	EiMatrix _current_states;

	StateBundle* _output_bundle;

	SSASimulator _simulator;
	KolmogorovComputerPtr _kolmogorov_computer;

	bool _newTrajectoriesSimilarToData(const parameters& params);
	bool _simulationsCloseToData(int time_point_nbr);

};

typedef boost::shared_ptr<ParticleEvaluatorInsight> evaluator_ptr;
} /* namespace INSIGHTv3 */

#endif /* PARTICLEEVALUATORINSIGHT_H_ */

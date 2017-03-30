/*
 *  SSASimulator.h
 *
 *  Created on: Jan 24, 2014
 *      Author: jan
 */

#ifndef SSASIMULATOR_H_
#define SSASIMULATOR_H_

#include <vector>

#include "INSIGHTv3.h"
#include "Model.h"
#include "StateBundle.h"

namespace INSIGHTv3 {

class SSASimulator {
public:
	SSASimulator(RngPtr rng);
	virtual ~SSASimulator();

	void setModel(Model* model);
	void simulateNTrajectoriesOneTimestep(const parameters& params,
			const double final_time, EiVector* times_of_last_fired_reaction,
			EiMatrix& Xs, EiMatrixRef outputs);
	void simulateTimestep(const parameters& params, const double final_time,
			double* time_of_last_fired_reaction, EiVectorRef X,
			EiVectorRef output);
	void simulateOneReaction(const parameters& params, double* time,
			EiVectorRef X);

	void simulateNTrajectoriesOneTimestep(Model& model,
			const parameters& params, const double final_time,
			EiVector* times_of_last_fired_reaction, EiMatrix& Xs,
			EiMatrixRef outputs);
	void simulateTimestep(Model& model, const parameters& params,
			const double final_time, double* time_of_last_fired_reaction,
			EiVectorRef X, EiVectorRef output);
	void simulateOneReaction(Model& model, const parameters& params,
			double* time, EiVectorRef X);
private:
	void _initializePropensities(Model& model);
	void _checkModelConsistency(const Model& model, const parameters& params,
			const propensities& props);

	void _simulateNTrajectoriesOneTimestep(Model& model,
			const parameters& params, const double final_time,
			EiVector* times_of_last_fired_reaction, EiMatrix& Xs,
			EiMatrixRef outputs);
	void _simulateTimestep(Model& model, const parameters& params,
			const double final_time, double* time_of_last_fired_reaction,
			EiVectorRef X, EiVectorRef output);
	void _simulateOneReaction(Model& model, const parameters& params,
			double* time, EiVectorRef X);

	DistUniform _dis;
	propensities* _props;
	Model* _model;
	bool _model_set;
};

} /* namespace INSIGHTv3 */

#endif /* SSASIMULATOR_H_ */

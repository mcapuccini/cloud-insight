/*
 * SSASimulator.cpp
 *
 *  Created on: Jan 24, 2014
 *      Author: jan
 *
 *      This class performs a Gillespi SSA simulation
 */

#include "SSASimulator.h"

#include <stddef.h>
#include <cmath>

#include "DimensionMissmatchException.h"
#include "IllegalArgumentException.h"

namespace INSIGHTv3 {
SSASimulator::SSASimulator(RngPtr rng) :
		_dis(*rng), _props(new propensities), _model(NULL), _model_set(
				false) {
}

SSASimulator::~SSASimulator() {
	delete _props;
}

void SSASimulator::setModel(Model* model) {
	_model = model;
	_props->resize(model->num_reactions);
	_model_set = true;
}

void SSASimulator::simulateNTrajectoriesOneTimestep(const parameters& params,
		const double final_time, EiVector* times_of_last_fired_reaction,
		EiMatrix& Xs, EiMatrixRef outputs) {

	if (!_model_set) {
		throw IllegalArgumentException("Model must be set first!");
	}

#ifdef DEBUG
	if (Xs.rows() != outputs.rows()) {
		std::ostringstream os;
		os << Xs.rows() << " trajectories need to be simulated, but only " << outputs.rows()
		<< "output trajectories are provided";
		throw DimensionMissmatchException(os.str().c_str());
	}
#endif

	_simulateNTrajectoriesOneTimestep(*_model, params, final_time,
			times_of_last_fired_reaction, Xs, outputs);
}

void SSASimulator::simulateTimestep(const parameters& params,
		const double final_time, double* time_of_last_fired_reaction,
		EiVectorRef X, EiVectorRef output) {
	if (!_model_set) {
		throw IllegalArgumentException("Model must be set first, or provided when calling the function simulateTimestep!");
	}
	_simulateTimestep(*_model, params, final_time, time_of_last_fired_reaction,
			X, output);
}

void SSASimulator::simulateOneReaction(const parameters& params, double* time,
		EiVectorRef X) {
	if (!_model_set) {
		throw IllegalArgumentException("Model must be set first!");
	}
	_simulateOneReaction(*_model, params, time, X);

}

void SSASimulator::simulateNTrajectoriesOneTimestep(Model& model,
		const parameters& params, const double final_time,
		EiVector* times_of_last_fired_reaction, EiMatrix& Xs,
		EiMatrixRef outputs) {

	_props->resize(model.num_reactions);

	_simulateNTrajectoriesOneTimestep(model, params, final_time,
			times_of_last_fired_reaction, Xs, outputs);

}

void SSASimulator::simulateTimestep(Model& model, const parameters& params,
		const double final_time, double* time_of_last_fired_reaction,
		EiVectorRef X, EiVectorRef output) {

#ifdef DEBUG
	model.checkDimensions(X, params, *_props);
#endif
	_props->resize(model.num_reactions);

	_simulateTimestep(model, params, final_time, time_of_last_fired_reaction, X,
			output);

}

void SSASimulator::simulateOneReaction(Model& model, const parameters& params,
		double* time, EiVectorRef X) {

	_props->resize(model.num_reactions);

	_simulateOneReaction(model, params, time, X);
}

void SSASimulator::_simulateNTrajectoriesOneTimestep(Model& model,
		const parameters& params, const double final_time,
		EiVector* times_of_last_fired_reaction, EiMatrix& Xs,
		EiMatrixRef outputs) {

	for (int trajectory_nbr = 0; trajectory_nbr < Xs.rows(); trajectory_nbr++) {

		double* time_point = &(*times_of_last_fired_reaction)(trajectory_nbr);
		EiVectorRef x = Xs.row(trajectory_nbr);
		EiVectorRef output = outputs.row(trajectory_nbr);

		_simulateTimestep(model, params, final_time, time_point, x, output);
	}

}

void SSASimulator::_simulateTimestep(Model& model, const parameters& params,
		const double final_time, double* time_of_last_fired_reaction,
		EiVectorRef X, EiVectorRef output) {

	if (final_time == 0) {

		model.setInitialConditions(X);
		model.createOutputState(X, params, output);
		*time_of_last_fired_reaction = 0;
	} else {

		double current_time = *time_of_last_fired_reaction;
		double waiting_time_till_next_reaction = 0.0;

		while (current_time + waiting_time_till_next_reaction <= final_time) {
			current_time += waiting_time_till_next_reaction;
			waiting_time_till_next_reaction = 0.0;
			simulateOneReaction(model, params, &waiting_time_till_next_reaction,
					X);
		}
		*time_of_last_fired_reaction = current_time;
		model.createOutputState(X, params, output);
	}
}

void SSASimulator::_simulateOneReaction(Model& model, const parameters& params,
		double* time, EiVectorRef X) {

	model.updatePropensities(X, params, _props);

	double prop_sum = _props->sum();

	double r1 = _dis();
	double r2 = _dis();
	double time_to_next_reaction = (1 / prop_sum) * log(1 / r1);

	size_t next_reaction_index = 0;
	double cumsum = (*_props)(0);
	while (cumsum <= r2 * prop_sum) {
		cumsum += (*_props)(next_reaction_index + 1);
		next_reaction_index++;
	}

	(*time) += time_to_next_reaction;

	model.updateState(next_reaction_index, X);
}

} /* namespace INSIGHTv3 */

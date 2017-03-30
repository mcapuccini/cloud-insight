/*
 * ParticleEvaluatorInsight.cpp
 *
 *  Created on: Jan 27, 2014
 *      Author: jan
 *
 *      This class is used to determine if a given particle is accepted or not. It is specifically designed to be used with the INSIGHT approach.
 *      Each instance of ParticleEvaluator is supposed to be used with one particular model and one particular set of data.
 *      If used with another SMC or inference scheme, this class would need to be reimplemented
 */

#include "ParticleEvaluatorInsight.h"

#include <stddef.h>
#include <cmath>
#include <map>

#include "StateBundle.h"
#include "DimensionMissmatchException.h"

namespace INSIGHTv3 {

ParticleEvaluatorInsight::ParticleEvaluatorInsight(Model& model,
		const std::vector<double> times, RngPtr rng,
		KolmogorovComputerPtr kolmogorov_computer) :
		_model(model), _times(times), _current_num_simulations(0), _current_tolerance(
				0.0), _times_of_last_fired_reaction(1), _current_states(1, 1), _output_bundle(
				NULL), _simulator(rng), _kolmogorov_computer(
				kolmogorov_computer) {
	_simulator.setModel(&model);
}

ParticleEvaluatorInsight::~ParticleEvaluatorInsight() {
	delete _output_bundle;
}

bool ParticleEvaluatorInsight::isParticleAcceptable(
		const parameters& particle) {

#ifdef DEBUG
	if (particle.size() != _model.num_params) {
		std::ostringstream os;
		os << "Provided particle has " << particle.size() << " parameters"
		<< ", but the model requires "<< _model.num_params << "!";
		throw DimensionMissmatchException(os.str().c_str());
	}
#endif

	bool new_trajectories_similar_to_data = _newTrajectoriesSimilarToData(
			particle);

	if (!new_trajectories_similar_to_data) {
		return false;
	}
	return true;
}

void ParticleEvaluatorInsight::setEvaluationParams(int num_simulations,
		double tolerance) {
	_current_num_simulations = num_simulations;
	_current_tolerance = tolerance;

	delete _output_bundle;
	_output_bundle = new StateBundle(_model.num_outputs, _times.size(),
			_current_num_simulations);
	_times_of_last_fired_reaction.resize(_current_num_simulations);
	_current_states.resize(_current_num_simulations, _model.num_species);
}

bool ParticleEvaluatorInsight::_newTrajectoriesSimilarToData(
		const parameters& params) {

	_times_of_last_fired_reaction.setZero();
	_current_states.setZero();

	for (size_t time_point_nbr = 0; time_point_nbr < _times.size();
			time_point_nbr++) {

		EiMatrixRef new_output_current_timepoint =
				_output_bundle->getStateBundle(0, time_point_nbr,
						_current_num_simulations, 1);

		_simulator.simulateNTrajectoriesOneTimestep(params,
				_times[time_point_nbr], &_times_of_last_fired_reaction,
				_current_states, new_output_current_timepoint);

		bool simulation_close_to_data = _simulationsCloseToData(time_point_nbr);
		if (!simulation_close_to_data) {
			return false;
		}
	}
	return true;
}

bool ParticleEvaluatorInsight::_simulationsCloseToData(int time_point_nbr) {
	for (int nbr_output = 0; nbr_output < _model.num_outputs; nbr_output++) {
		double distance = _kolmogorov_computer->distTwoSample(
				_output_bundle->getSingleStateForTimepoint(0, time_point_nbr,
						_current_num_simulations, nbr_output),
				time_point_nbr + nbr_output);
		std::cout << "distance: " << distance << std::endl;
		if (distance >= _current_tolerance) {
			return false;
		}
	}
	return true;
}

} /* namespace INSIGHTv3 */

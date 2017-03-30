/*
 * StateBundle.cpp
 *
 *  Created on: Jan 28, 2014
 *      Author: jan
 *
 *      This class manages a bundle of states of a dynamical system and allows to acces subsets of these states
 */

#include "StateBundle.h"

namespace INSIGHTv3 {

StateBundle::StateBundle(int num_states, int num_timepoints,
		int num_trajectories) :
		num_states(num_states), num_timepoints(num_timepoints), num_trajectories(
				num_trajectories) {
	_trajectories = new EiMatrix(num_trajectories,
			num_states * num_timepoints);

}

StateBundle::~StateBundle() {
	// TODO Auto-generated destructor stub
}

EiVectorRow StateBundle::getState(int trajectory, int timepoint) {

	return EiVectorRow(*_trajectories, trajectory, timepoint * num_states,
			1, num_states);
}

EiSubMatrix StateBundle::getStateBundle(int start_trajectory,
		int start_timepoint, int num__selected_trajectories,
		int num_selected_timepoints) {
	return _trajectories->block(start_trajectory, start_timepoint * num_states,
			num__selected_trajectories, num_selected_timepoints * num_states);
}

EiVectorCol StateBundle::getSingleStateForTimepoint(int start_trajectory,
		int timepoint, int num_selected_trajectories, int species_nbr) {

	return EiVectorCol(*_trajectories, start_trajectory,
			timepoint * num_states + species_nbr, num_selected_trajectories, 1);

}
} /* namespace INSIGHTv3 */

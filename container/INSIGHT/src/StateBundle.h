/*
 * StateBundle.h
 *
 *  Created on: Jan 28, 2014
 *      Author: jan
 */

#ifndef STATEBUNDLE_H_
#define STATEBUNDLE_H_

#include <MatrixTypes.h>

namespace INSIGHTv3 {

class StateBundle {
public:
	StateBundle(int num_states, int num_timepoints, int num_trajectories);
	virtual ~StateBundle();

	const int num_states;
	const int num_timepoints;
	const int num_trajectories;

	EiVectorRow getState(int trajectory, int timepoint);
	EiSubMatrix getStateBundle(int start_trajectory, int start_timepoint,
			int num_trajectories, int num_timepoints);
	EiVectorCol getSingleStateForTimepoint(int start_trajectory,
			int timepoint, int num_trajectories, int species_nbr);

private:
	EiMatrix* _trajectories;
};

} /* namespace INSIGHTv3 */

#endif /* STATEBUNDLE_H_ */

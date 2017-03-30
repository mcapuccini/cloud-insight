/*
 * IterationLogger.h
 *
 *  Created on: Aug 15, 2013
 *      Author: jan
 */

#ifndef ITERATIONLOGGER_H_
#define ITERATIONLOGGER_H_

#include <stdio.h>
#include <time.h>

namespace INSIGHTv3 {

class IterationLogger {
public:
	IterationLogger(int print_frequency);
	~IterationLogger();

	void print(int num_sampled_particles, int num_accepted_particles,
			double weight, clock_t tic);
	void setParams(double epsilon, int s, int num_particles);
	double getAcceptanceRate() {
		return _acceptance_rate;
	}
	;

private:
	int _print_frequency;
	double _epsilon;
	int _s;
	int _num_particles;
	double _acceptance_rate;
};

} /* namespace INSIGHTv3 */

#endif /* ITERATIONLOGGER_H_ */

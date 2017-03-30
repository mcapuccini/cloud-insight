/*
 * IterationLogger.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: jan
 *
 *      This class loggs the progress of the INSIGHT algorithm and prints the information.
 */

#include "IterationLogger.h"

#include <iostream>
#include <cmath>

namespace INSIGHTv3 {

IterationLogger::IterationLogger(int print_frequency) :
		_print_frequency(print_frequency) {
	_epsilon = 0.0;
	_s = 0;
	_num_particles = 0;
	_acceptance_rate = 0.0;
}

IterationLogger::~IterationLogger() {

}

void IterationLogger::print(int num_sampled_particles,
		int num_accepted_particles, double weight, clock_t tic) {
	if (num_accepted_particles % _print_frequency == 0) {
		std::cout << "i = " << num_accepted_particles << ", S = " << _s
				<< ", epsilon = " << _epsilon << ", weight = " << weight
				<< std::endl;
	}
	if (num_accepted_particles % 100 == 0) {
		_acceptance_rate = ((double) num_accepted_particles)
								/ ((double) num_sampled_particles);
		std::cout << std::endl << std::endl << "Particle acceptance rate: "
				<< _acceptance_rate << std::endl;

		// Compute estimate of remaining time
		clock_t toc2 = clock();

		double sec = ((double) (toc2 - tic) / CLOCKS_PER_SEC);
		double eta = (sec / ((double) num_accepted_particles))
				* (_num_particles - num_accepted_particles);
		int hrs = floor(eta / 3600);
		eta = eta - hrs * 3600;
		int min = floor(eta / 60);
		eta = eta - min * 60;
		std::cout << "Estimated time remaining: " << hrs << " hrs " << min << " min " << eta << " sec "<< std::endl << std::endl;;
	}
}
void IterationLogger::setParams(double epsilon, int s, int num_particles) {
	_s = s;
	_epsilon = epsilon;
	_num_particles = num_particles;
}

}
/* namespace INSIGHTv3 */

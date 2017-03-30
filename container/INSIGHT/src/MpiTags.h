/*
 * Mpi_tags.h
 *
 *  Created on: Feb 11, 2014
 *      Author: jan
 */

#ifndef MPITAGS_H_
#define MPITAGS_H_

#include <mpi.h>
namespace INSIGHTv3 {

enum MPI_INSTRUCTION {
	INSTRUCTION,
	WORKTAG,
	DIETAG,
	PARTICLE,
	WEIGHT,
	ITERATIONS,
	NUM_SIMULATIONS_RUNS,
	TOLERANCE,
	SAMPLER_UPDATE,
	SAMPLER_SIZE,
	SAMPLER,
	EVALUATOR_UPDATE
};

}

#endif /* MPITAGS_H_ */

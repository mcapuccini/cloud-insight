/*
 * InsightMpiWorker.cpp
 *
 *  Created on: Feb 12, 2014
 *      Author: jan
 *
 *      This is the worker class for the InsightIterationMpi. It does all the work that is distributed and sends back the sampled particles.
 */

#include "InsightMpiWorker.h"

#include <iostream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace INSIGHTv3 {

InsightMpiWorker::InsightMpiWorker(size_t my_rank, size_t num_parameters,
		evaluator_ptr evaluator, sampler_ptr prior_sampler) :
		_my_rank(my_rank), _num_parameters(num_parameters), _sampler(NULL), _evaluator(
				evaluator), _particle(new double[num_parameters]), _particle_ei_vector(
				parameters(num_parameters)), _prior_sampler(prior_sampler) {

}

InsightMpiWorker::~InsightMpiWorker() {
}

void InsightMpiWorker::run() {

	MPI::Status status;
	MPI_INSTRUCTION instruction(INSTRUCTION);

	while (true) {

		MPI::COMM_WORLD.Recv(&instruction, 1, MPI::INT, 0, INSTRUCTION);
		switch (instruction) {

		case DIETAG:
			std::cout << "This is process " << _my_rank << " , I am exiting."
					<< std::endl;
			return;
			break;

		case SAMPLER_UPDATE:
			_updateSampler();
			break;

		case EVALUATOR_UPDATE:

			_updateEvaluator();
			break;

		case WORKTAG:
			_provideParticle();
			break;

		default:
			break;
		}
	}
	return;
}

void InsightMpiWorker::_updateSampler() {
	int particle_sampler_size = 0;
	MPI::COMM_WORLD.Recv(&particle_sampler_size, 1, MPI::INT, 0, SAMPLER_SIZE);

	char particle_sampler_char_ptr[particle_sampler_size];
	MPI::COMM_WORLD.Recv(particle_sampler_char_ptr, particle_sampler_size,
			MPI::CHAR, 0, SAMPLER);

	std::string particle_sampler_str(particle_sampler_char_ptr,
			particle_sampler_char_ptr + particle_sampler_size);
	std::stringstream stream(particle_sampler_str);
	boost::archive::text_iarchive iar(stream);

	_sampler = (ParticleSampler*) malloc(particle_sampler_size);
	iar >> _sampler;
	int seed = time(NULL) + _my_rank;
	_sampler->setSeed(seed);
}

void InsightMpiWorker::_provideParticle() {
	bool particle_accepted = false;
	int num_sampled_particles = 0;
	while (!particle_accepted) {
		_sampler->sampleParticle(&_particle_ei_vector);
		num_sampled_particles++;
		particle_accepted = _evaluator->isParticleAcceptable(
				_particle_ei_vector);
	}
	Eigen::Map<EiVector>(_particle, _num_parameters) = _particle_ei_vector;
	double weight = _prior_sampler->getLikelihoodForParticle(
			_particle_ei_vector)
			/ _sampler->getLikelihoodForParticle(_particle_ei_vector);

	MPI::COMM_WORLD.Send(_particle, _num_parameters, MPI::DOUBLE, 0, PARTICLE);
	MPI::COMM_WORLD.Send(&weight, 1, MPI::DOUBLE, 0, WEIGHT);
	MPI::COMM_WORLD.Send(&num_sampled_particles, 1, MPI::INT, 0, ITERATIONS);
}

void InsightMpiWorker::_updateEvaluator() {
	int s = 0;
	double epsilon = 0.0;
	MPI::COMM_WORLD.Recv(&s, 1, MPI::INT, 0, NUM_SIMULATIONS_RUNS);
	MPI::COMM_WORLD.Recv(&epsilon, 1, MPI::DOUBLE, 0, TOLERANCE);
	_evaluator->setEvaluationParams(s, epsilon);

}

} /* namespace INSIGHTv3 */

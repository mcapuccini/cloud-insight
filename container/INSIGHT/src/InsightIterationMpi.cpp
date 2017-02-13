/*
 * InsightIterationMpi.cpp
 *
 *  Created on: Feb 11, 2014
 *      Author: jan
 *
 *      This class represents one iteration of the INSIGHT algorithm for parallel use. It samples particles, simulates the system and accepts the particle if the simulations results
 *      are similar to the observed original data. This class should exists only in the main process with rank 0. It also comunicates with the mpi worker It does that until it has sufficient particles.
 */

#include "InsightIterationMpi.h"

#include <boost/serialization/export.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "MpiTags.h"
#include "ParticleSampler.h"
#include "DistributionKernel.h"
#include "KernelParticleSampler.h"
#include "UniformSampler.h"
#include "UniformKernel.h"
#include "ScaledUniformKernel.h"

BOOST_CLASS_EXPORT_GUID(INSIGHTv3::ParticleSampler, "ParticleSampler")
BOOST_CLASS_EXPORT_GUID(INSIGHTv3::DistributionKernel, "DistributionKernel")
BOOST_CLASS_EXPORT_GUID(INSIGHTv3::KernelParticleSampler,
		"KernelParticleSampler")
BOOST_CLASS_EXPORT_GUID(INSIGHTv3::UniformSampler, "UniformSampler")
BOOST_CLASS_EXPORT_GUID(INSIGHTv3::UniformKernel, "UniformKernel")
BOOST_CLASS_EXPORT_GUID(INSIGHTv3::ScaledUniformKernel, "ScaledUniformKernel")
namespace INSIGHTv3 {

InsightIterationMpi::InsightIterationMpi(int num_tasks,
		sampler_ptr prior_sampler) :
		InsightIteration(prior_sampler), _num_tasks(num_tasks), _num_new_sampled_particles(
				0), _weight(0), _particle(
		NULL), _num_parameters(0) {
	// TODO Auto-generated constructor stub

}

InsightIterationMpi::~InsightIterationMpi() {
	// TODO Auto-generated destructor stub
}

void InsightIterationMpi::doIteration(int num_particles, int num_parameters,
		sampler_ptr sampler, evaluator_ptr evaluator, ParticleSet* particle_set,
		IterationLogger* logger) {

	clock_t tic = clock();
	_num_parameters = num_parameters;
	_particle = new double[num_parameters];
	int num_requested_particles = 0;
	int num_received_particles = 0;
	int num_sampled_particles = 0;

	MPI::Status status;
	_updateParticleSampler(sampler);
	_updateParticleEvaluator(evaluator);

	for (int rank = 1; rank < _num_tasks; rank++) {
		_requestNextParticle(rank);
		num_requested_particles++;
	}

	while (num_requested_particles < num_particles) {

		_receiveParticle(status, particle_set);
		num_sampled_particles += _num_new_sampled_particles;
		num_received_particles++;
		logger->print(num_sampled_particles, num_received_particles, _weight,
				tic);

		_requestNextParticle(status.Get_source());
		num_requested_particles++;
	}

	for (int rank = 1; rank < _num_tasks; rank++) {
		_receiveParticle(status, particle_set);
		num_sampled_particles += _num_new_sampled_particles;
		num_received_particles++;
		logger->print(num_sampled_particles, num_received_particles, _weight,
				tic);
	}

	delete[] _particle;
}

void InsightIterationMpi::_requestNextParticle(size_t dest_process) {
	MPI_INSTRUCTION instruction(WORKTAG);
	MPI::COMM_WORLD.Send(&instruction, 1, MPI_INT, dest_process, INSTRUCTION);
}

void InsightIterationMpi::_receiveParticle(MPI::Status& status,
		ParticleSet* particle_set) {
	MPI::COMM_WORLD.Recv(_particle, _num_parameters, MPI::DOUBLE,
			MPI::ANY_SOURCE, PARTICLE, status);
	MPI::COMM_WORLD.Recv(&_weight, 1, MPI::DOUBLE, status.Get_source(), WEIGHT);
	MPI::COMM_WORLD.Recv(&_num_new_sampled_particles, 1, MPI::INT,
			status.Get_source(), ITERATIONS);
	particle_set->acceptParticle(_particle, _weight, _num_parameters);
}

void InsightIterationMpi::_updateParticleSampler(sampler_ptr sampler) {
	MPI_INSTRUCTION instruction(SAMPLER_UPDATE);
	std::stringstream stream;
	boost::archive::text_oarchive oar(stream);
	ParticleSampler* _part_s = sampler.get();
	oar & _part_s;
	size_t particle_sampler_size = stream.str().size();
	std::string particle_sampler_str = stream.str();

	for (int rank = 1; rank < _num_tasks; rank++) {
		MPI::COMM_WORLD.Send(&instruction, 1, MPI::INT, rank, INSTRUCTION);
		MPI::COMM_WORLD.Send(&particle_sampler_size, 1, MPI::INT, rank,
				SAMPLER_SIZE);
		MPI::COMM_WORLD.Send(particle_sampler_str.c_str(),
				particle_sampler_size, MPI::CHAR, rank, SAMPLER);
	}
}

void InsightIterationMpi::_updateParticleEvaluator(evaluator_ptr evaluator) {
	double epsilon = evaluator->getTolerance();
	int s = evaluator->getNumSimulations();

	MPI_INSTRUCTION instruction(EVALUATOR_UPDATE);
	for (int rank = 1; rank < _num_tasks; rank++) {
		MPI::COMM_WORLD.Send(&instruction, 1, MPI::INT, rank, INSTRUCTION);
		MPI::COMM_WORLD.Send(&s, 1, MPI::INT, rank, NUM_SIMULATIONS_RUNS);
		MPI::COMM_WORLD.Send(&epsilon, 1, MPI::DOUBLE, rank, TOLERANCE);
	}
}
} /* namespace INSIGHTv3 */

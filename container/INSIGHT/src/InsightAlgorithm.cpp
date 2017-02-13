/*
 * InsightAlgorithm.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: jan
 *
 *      This class represents the actual INSIGHT algorithm. It calls all SMC iterations, updated all necessary parameters and writes the output.
 */

#include <InsightAlgorithm.h>

namespace INSIGHTv3 {

InsightAlgorithm::InsightAlgorithm(InsightIteration& insightIteration,
		sampler_factory_ptr sampler_factory, evaluator_ptr particle_evaluator,
		writer_ptr writer, ToleranceProviderPtr tolerance_provider,
		double final_tolerance, int num_particles, int num_parameters) :
		_insightIteration(insightIteration), _sampler_factory(sampler_factory), _particle_evaluator(
				particle_evaluator), _writer(writer), _tolerance_provider(
				tolerance_provider), _final_tolerance(final_tolerance), _num_particles(
				num_particles), _num_parameters(num_parameters) {
}

InsightAlgorithm::InsightAlgorithm(InsightIteration& insightIteration,
		sampler_factory_ptr sampler_factory, evaluator_ptr particle_evaluator,
		writer_ptr writer, ToleranceProviderPtr tolerance_provider,
		double final_tolerance, int num_particles, int num_parameters,
		std::vector<double> prev_acceptance_rates,
		std::vector<double> prev_tolerances,
		std::vector<int> prev_num_simulations) :
		_insightIteration(insightIteration), _sampler_factory(sampler_factory), _particle_evaluator(
				particle_evaluator), _writer(writer), _tolerance_provider(
				tolerance_provider), _final_tolerance(final_tolerance), _num_particles(
				num_particles), _num_parameters(num_parameters), _acceptance_rates(
				prev_acceptance_rates), _tolerances(prev_tolerances), _num_simulations(
				prev_num_simulations) {
}

InsightAlgorithm::~InsightAlgorithm() {
	// TODO Auto-generated destructor stub
}

void InsightAlgorithm::run(sampler_ptr first_sampler, IterationLogger logger) {
	double current_tolerance = 1;
	int current_num_simulations = 0;

	ParticleSet* prev_particle_set = new ParticleSet(_num_particles);
	ParticleSet* current_particle_set;

	sampler_ptr sampler = first_sampler;
	int iteration_nbr = 1;

	while (current_tolerance > _final_tolerance) {
		time_t tic = clock();
		std::cout << "Iteration " << iteration_nbr << " is starting."
				<< std::endl;

		//current_particle_set = new ParticleSet(_num_particles);
		_updateTolerance(&current_num_simulations, &current_tolerance, &logger);

		break;

		_insightIteration.doIteration(_num_particles, _num_parameters, sampler,
				_particle_evaluator, current_particle_set, &logger);

		current_particle_set->normalizeWeights();
		delete prev_particle_set;
		prev_particle_set = current_particle_set;

		_tolerance_provider->next();
		int s_for_sampler = _tolerance_provider->getCurrentSimulationNbr();
		double tol_for_sampler = _tolerance_provider->getCurrentTolerance();

		sampler = _sampler_factory->createSampler(*prev_particle_set,
				tol_for_sampler, s_for_sampler);

		time_t toc = clock();

		_num_simulations.push_back(current_num_simulations);
		_tolerances.push_back(current_tolerance);
		_acceptance_rates.push_back(logger.getAcceptanceRate());
		_writer->writeOutput(*current_particle_set, current_num_simulations);
		_writer->writeAcceptanceRate(_num_simulations, _tolerances,
				_acceptance_rates);

		std::cout << std::endl << "Iteration " << iteration_nbr << " needed "
				<< (toc - tic) / CLOCKS_PER_SEC << " seconds\n" << std::endl;
		iteration_nbr++;
	}
	delete prev_particle_set;

}

void InsightAlgorithm::_updateTolerance(int* current_num_simulations,
		double* current_tolerance, IterationLogger* logger) {
	//*current_num_simulations = _tolerance_provider->getCurrentSimulationNbr();
	//*current_tolerance = _tolerance_provider->getCurrentTolerance();
	logger->setParams(*current_tolerance, *current_num_simulations,
			_num_particles);
	_particle_evaluator->setEvaluationParams(*current_num_simulations,
			*current_tolerance);
}
} /* namespace INSIGHTv3 */

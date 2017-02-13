/*
 * InsightAlgorithm.h
 *
 *  Created on: Jan 30, 2014
 *      Author: jan
 */

#ifndef INSIGHTALGORITHM_H_
#define INSIGHTALGORITHM_H_

#include "FixedSequenceToleranceProvider.h"
#include "InsightIteration.h"
#include "IterationLogger.h"
#include "InsightParticleSamplerFactory.h"
#include "InsightOutputWriter.h"

namespace INSIGHTv3 {

class InsightAlgorithm {
public:
	InsightAlgorithm(InsightIteration& insightIteration,
			sampler_factory_ptr sampler_factory,
			evaluator_ptr particle_evaluator, writer_ptr writer,
			ToleranceProviderPtr tolerance_provider, double final_tolerance,
			int num_particles, int num_parameters);
	InsightAlgorithm(InsightIteration& insightIteration,
			sampler_factory_ptr sampler_factory,
			evaluator_ptr particle_evaluator, writer_ptr writer,
			ToleranceProviderPtr tolerance_provider, double final_tolerance,
			int num_particles, int num_parameters,
			std::vector<double> prev_acceptance_rates,
			std::vector<double> prev_tolerances, std::vector<int> prev_num_simulations);
	virtual ~InsightAlgorithm();

	void run(sampler_ptr first_sampler, IterationLogger logger);
    void _updateTolerance(int* current_num_simulations,
            double* current_tolerance, IterationLogger* logger);

private:
	InsightIteration& _insightIteration;
	sampler_factory_ptr _sampler_factory;
	evaluator_ptr _particle_evaluator;
	writer_ptr _writer;
	ToleranceProviderPtr _tolerance_provider;
	double _final_tolerance;
	int _num_particles;
	int _num_parameters;

	std::vector<double> _acceptance_rates;
	std::vector<double> _tolerances;
	std::vector<int> _num_simulations;

};

} /* namespace INSIGHTv3 */

#endif /* INSIGHTALGORITHM_H_ */

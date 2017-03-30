/*
 * FixedSequenceToleranceProvider.h
 *
 *  Created on: Jan 30, 2014
 *      Author: jan
 */

#ifndef FIXEDSEQUENCETOLERANCEPROVIDER_H_
#define FIXEDSEQUENCETOLERANCEPROVIDER_H_

#include <vector>
#include <map>

#include "InsightToleranceProvider.h"
#include "Model.h"
#include "KolmogorovComputer.h"

namespace INSIGHTv3 {

class FixedSequenceToleranceProvider : public INSIGHTv3::InsightToleranceProvider{
public:
	FixedSequenceToleranceProvider(int s, MODELS model, KolmogorovComputerPtr kolmogorov_computer);
	virtual ~FixedSequenceToleranceProvider();

	int getCurrentSimulationNbr();
	double getCurrentTolerance();
	void next();

private:
	std::vector<int> _num_simulations;
	std::vector<int>::iterator _current_simulation_it;
	std::map<int, double> _tolerance_by_s;

	KolmogorovComputerPtr _kolmogorov_computer;


	void _initForModel(MODELS model);
	void _fillForBirthDeath();
	void _fillForLacGfp();
};

} /* namespace INSIGHTv3 */

#endif /* FIXEDSEQUENCETOLERANCEPROVIDER_H_ */

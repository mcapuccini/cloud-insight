/*
 * LacGfp7.h
 *
 *  Created on: Feb 18, 2014
 *      Author: jan
 */

#ifndef LACGFP7_H_
#define LACGFP7_H_

#include <ModelFl.h>

#include <Eigen/Dense>

namespace INSIGHTv3 {

class LacGfp7Model: public INSIGHTv3::ModelFl {
public:
	LacGfp7Model(const std::vector<EiVector> background_fluorescence, RngPtr r,
			std::vector<int> output_states);
	virtual ~LacGfp7Model();

protected:
	void _updatePropensities(const state& X, const parameters& params,
			propensities* props) const;
	void _updateState(const size_t reaction_index, EiVectorRef X) const;
	void _createOutputState(const state& X, const parameters& params,
			EiVectorRef output);
	void _setInitialConditions(EiVectorRef X);

private:

	static const size_t _NUM_SPECIES = 9;
	static const size_t _NUM_PARAMETERS = 18;
	static const size_t _NUM_REACTIONS = 18;
	static const MODELS _MODEL = MODLE_LAC_GFP;

	DistUniformInt _dist_uniform_int_X0;
	DistUniformInt _dist_uniform_int_X1;
	DistUniformInt _dist_uniform_int_X5;

};

} /* namespace INSIGHTv3 */

#endif /* LACGFP7_H_ */

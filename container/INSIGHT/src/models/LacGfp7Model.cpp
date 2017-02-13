/*
 * LacGfp7.cpp
 *
 *  Created on: Feb 18, 2014
 *      Author: jan
 *
 *      Model for the Lac Gfp system. For details see Lillacci & Khammash 2013
 */

#include "LacGfp7Model.h"

namespace INSIGHTv3 {

LacGfp7Model::LacGfp7Model(
		const std::vector<EiVector> background_fluorescence, RngPtr r, std::vector<int> output_states) :
		ModelFl(background_fluorescence, r, _NUM_SPECIES,
				_NUM_PARAMETERS + 2 * output_states.size(), _NUM_REACTIONS,
				"Lac-GFP construct model v7 (LACGFP7)", _MODEL, output_states), _dist_uniform_int_X0(
				*_r, UniformIntMapping(0, 5)), _dist_uniform_int_X1(*_r,
				UniformIntMapping(0, 10)), _dist_uniform_int_X5(*_r,
				UniformIntMapping(0, 20)) {

}

LacGfp7Model::~LacGfp7Model() {
// TODO Auto-generated destructor stub
}

void LacGfp7Model::_updatePropensities(const state& X, const parameters& params,
		propensities* props) const {
	// Recover species from X vector
	double X1 = X(0);
	double X2 = X(1);
	double X3 = X(2);
	double X4 = X(3);
	double X5 = X(4);
	double X6 = X(5);
	double X7 = X(6);
	double X8 = X(7);
	double X9 = X(8);

	// Parameter recovery statements
	int begin_index = 2 * output_states.size(); // the first parameters are reserved for the mean and std of the fluourescence
	double k1 = params(begin_index);
	double k2 = params(begin_index + 1);
	double k3 = params(begin_index + 2);
	double k4 = params(begin_index + 3);
	double k5 = params(begin_index + 4);
	double k6 = params(begin_index + 5);
	double k7 = params(begin_index + 6);
	double k8 = params(begin_index + 7);
	double k9 = params(begin_index + 8);
	double k10 = params(begin_index + 9);
	double k11 = params(begin_index + 10);
	double k12 = params(begin_index + 11);
	double k13 = params(begin_index + 12);
	double k14 = params(begin_index + 13);
	double k15 = params(begin_index + 14);
	double k16 = params(begin_index + 15);
	double k17 = params(begin_index + 16);
	double k18 = params(begin_index + 17);

	// Input recovery statements
	double u1 = 10.0;

	// Propensity evaluation statements
	(*props)(0) = k1;
	(*props)(1) = k2 * X1;
	(*props)(2) = k3 * X1;
	(*props)(3) = (k4 + k5 * u1) * X2;
	(*props)(4) = k6 * X2 * (X2 - 1);
	(*props)(5) = k7 * X3;
	(*props)(6) = k8 * X3 * X4;
	(*props)(7) = k9 * X5;
	(*props)(8) = k10 * X5 * (X5 - 1);
	(*props)(9) = k11 * X6;
	(*props)(10) = k12 * X4;
	(*props)(11) = k13 * X5;
	(*props)(12) = k14 * X6;
	(*props)(13) = k15 * X7;
	(*props)(14) = k16 * X7;
	(*props)(15) = k17 * X8;
	(*props)(16) = k18 * X8;
	(*props)(17) = k17 * X9;

}

void LacGfp7Model::_updateState(const size_t reaction_index,
		EiVectorRef X) const {
	switch (reaction_index) {

	case 0:

		X(0) = X(0) + 1;
		break;

	case 1:
		X(0) = X(0) - 1;
		break;

	case 2:
		X(1) = X(1) + 1;
		break;

	case 3:
		X(1) = X(1) - 1;
		break;

	case 4:
		X(1) = X(1) - 2;
		X(2) = X(2) + 1;
		break;

	case 5:
		X(1) = X(1) + 2;
		X(2) = X(2) - 1;
		break;

	case 6:
		X(2) = X(2) - 1;
		X(3) = X(3) - 1;
		X(4) = X(4) + 1;
		break;

	case 7:
		X(2) = X(2) + 1;
		X(3) = X(3) + 1;
		X(4) = X(4) - 1;
		break;

	case 8:
		X(4) = X(4) - 2;
		X(5) = X(5) + 1;
		break;

	case 9:
		X(4) = X(4) + 2;
		X(5) = X(5) - 1;
		break;

	case 10:
		X(6) = X(6) + 1;
		break;

	case 11:
		X(6) = X(6) + 1;
		break;

	case 12:
		X(6) = X(6) + 1;
		break;

	case 13:
		X(6) = X(6) - 1;
		break;

	case 14:
		X(7) = X(7) + 1;
		break;

	case 15:
		X(7) = X(7) - 1;
		break;

	case 16:
		X(7) = X(7) - 1;
		X(8) = X(8) + 1;
		break;

	case 17:
		X(8) = X(8) - 1;
		break;
	}
}

void LacGfp7Model::_createOutputState(const state& X, const parameters& params,
		EiVectorRef output) {

	EiConstVectorRef mu = params.head(output_states.size());
	EiConstVectorRef std = params.block(output_states.size(), 0,
			output_states.size(), 1);

	_addFlLevel(X, mu, std, output);
}

void LacGfp7Model::_setInitialConditions(EiVectorRef X) {
	X(0) = _dist_uniform_int_X0();
	X(1) = _dist_uniform_int_X1();
	X(2) = 0;
	X(3) = 0;
	X(4) = 0;
	X(5) = 50 + _dist_uniform_int_X5();
	X(6) = 0;
	X(7) = 0;
	X(8) = 0;
}
} /* namespace INSIGHTv3 */

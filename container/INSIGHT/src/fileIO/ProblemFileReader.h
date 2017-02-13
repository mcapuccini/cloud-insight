/*
 * ProblemFileReader.h
 *
 *  Created on: Jan 30, 2014
 *      Author: jan
 */

#ifndef PROBLEMFILEREADER_H_
#define PROBLEMFILEREADER_H_

#include <string>
#include <vector>

#include <boost/property_tree/ptree.hpp>

#include "MatrixTypes.h"
#include "Model.h"
#include "INSIGHTv3.h"
#include "ModelDescription.h"

namespace INSIGHTv3 {

typedef boost::property_tree::ptree bt_ptree;
enum DATA_MODE {
	FCS, TEXT,
};

class ProblemFileReader {
public:
	ProblemFileReader();
	virtual ~ProblemFileReader();

	ModelDescription read(RngPtr r, std::string);

protected:
	void _createModel(RngPtr r, bt_ptree pt, std::string problem_file_path);
	void _createPriors(bt_ptree pt);
	void _createData(bt_ptree pt, std::string);

	Model* _model;
	std::vector<EiVector>* _original_data;
	EiVector* _lower_bounds;
	EiVector* _upper_bounds;
	std::vector<double>* _times;
};

} /* namespace INSIGHTv3 */

#endif /* PROBLEMFILEREADER_H_ */

/*
 * DataReader.h
 *
 *  Created on: Sep 27, 2012
 *      Author: jan
 */

#ifndef DATAREADER_H_
#define DATAREADER_H_

#include <vector>
#include <string>

#include "MatrixTypes.h"

namespace INSIGHTv3 {

class DataReader {

public:
	DataReader();
	virtual ~DataReader();
	static void readDataOneFile(std::string data_file, int num_timepoints,
			int num_samples, int num_measured_species,
			std::vector<EiVector>* original_data);
	static EiVector readFCSFile(std::string data_file, int sample_size,
			int fcs_parameter);
	static EiVector readTextFile(std::string data_file, int sample_size);

};

} /* namespace INSIGHTv3 */

#endif /* DATAREADER_H_ */

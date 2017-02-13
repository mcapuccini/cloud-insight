/*
 * BGReader.h
 *
 *  Created on: Sep 27, 2012
 *      Author: jan
 */

#ifndef BGREADER_H_
#define BGREADER_H_

#include <stddef.h>
#include <string>

#include "MatrixTypes.h"

namespace INSIGHTv3 {

class BGReader {
public:
	BGReader();
	virtual ~BGReader();

	static EiVector readSingleBGFile(std::string bg_file_path, size_t num_samples,
			size_t fcs_parameter_nbr);
private:

};

} /* namespace INSIGHTv3 */

#endif /* BGREADER_H_ */

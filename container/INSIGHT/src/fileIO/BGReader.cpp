/*
 * BGReader.cpp
 *
 *  Created on: Sep 27, 2012
 *      Author: jan
 *
 *      This class is used to read the background fluorescence if provided. It is assumed, that the background fluorescence is given in a fcs file
 */

#include "fileIO/BGReader.h"

#include <cstdlib>
#include <iostream>
#include <string>

#include "fileIO/FcsProcessor.h"
#include "IllegalArgumentException.h"

namespace INSIGHTv3 {

BGReader::BGReader() {
}

BGReader::~BGReader() {

}

EiVector BGReader::readSingleBGFile(std::string bg_file, size_t num_samples,
		size_t fcs_parameter_nbr) {

	EiVector bg_single_file(num_samples);

	// Open the FCS file
	std::ifstream fcs_file;
	fcs_file.open((const char*) bg_file.c_str(), std::ifstream::in);
	if (!fcs_file.is_open()) {
		std::ostringstream os;
		os << "Background file " << bg_file << " could not be read!"
				<< std::endl;
		throw IllegalArgumentException(os.str().c_str());
	}

	// Read the FCS file header
	float ver;
	size_t dos;
	size_t tot;
	size_t par;
	bool small_endianess;

	FcsProcessor::readHeader(&fcs_file, &ver, &dos, &small_endianess, &tot,
			&par);

	// Initialize read buffer
	float fbuf[par];

	// Position the file cursor at the beginning of the data segment
	fcs_file.seekg(dos);

	for (size_t m = 0; m < num_samples; m++) {
		// Keep reading until the desired parameter is > 1.1
		do {
			FcsProcessor::read_parameters(&fcs_file, small_endianess, par,
					fbuf);
		} while (fbuf[fcs_parameter_nbr - 1] <= 1.1);

		bg_single_file(m) = fbuf[fcs_parameter_nbr - 1];

	}
	// Close the FCS file
	fcs_file.close();

	return bg_single_file;

}

} /* namespace INSIGHTv3 */

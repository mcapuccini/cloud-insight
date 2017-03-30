/*
 * FscProcessor.h
 *
 *  Created on: Nov 13, 2012
 *      Author: jan
 */

#ifndef FSCPROCESSOR_H_
#define FCSPROCESSOR_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>

class FcsProcessor {
public:
	FcsProcessor();
	virtual ~FcsProcessor();

	static void readHeader(std::ifstream* fcs_file, float* ver,
			size_t* data_starting_byte, bool* small_endianess,
			size_t* num_total_events, size_t* num_parameters);
	static void read_parameters(std::ifstream* fcs_file, bool small_endianness, size_t num_parameters, float * parameters );

private:
	static int seek_to_keyword(std::ifstream* fcs_file, std::string keyword);
	static int num_chars_till_keyword(std::ifstream* fcs_file, std::string keyword);
};

#endif /* FSCPROCESSOR_H_ */

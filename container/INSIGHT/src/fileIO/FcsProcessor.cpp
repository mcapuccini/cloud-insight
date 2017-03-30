/*
 * FcsProcessor.cpp
 *
 *  Created on: Nov 13, 2012
 *      Author: jan
 *
 *      This class handles the reading of the fcs files.
 */

#include "FcsProcessor.h"

#include <stdlib.h>

FcsProcessor::FcsProcessor() {

}

FcsProcessor::~FcsProcessor() {

}

void FcsProcessor::readHeader(std::ifstream* fcs_file, float* ver,
		size_t* data_starting_byte, bool* small_endianess,
		size_t* num_total_events, size_t* num_parameters) {

	char buf[256];

	// read version
	fcs_file->seekg(3);
	fcs_file->read(buf, 3);
	*ver = atof(buf);
	memset(buf, 0, sizeof buf);

	std::string delimiter;
	if (*ver < 3.0) {
		delimiter = "\\";
	} else {
		delimiter = "|";
	}
	//read data starting byte
	fcs_file->seekg(26);
	fcs_file->read(buf, 8);
	*data_starting_byte = atol(buf);
	memset(buf, 0, sizeof buf);

	//read number total events
	seek_to_keyword(fcs_file, "$TOT");
	fcs_file->seekg((int) fcs_file->tellg() + 4 + 1);
	int length = num_chars_till_keyword(fcs_file, delimiter);
	fcs_file->read(buf, length);
	*num_total_events = atol(buf);
	memset(buf, 0, sizeof buf);

	//read number parameters
	seek_to_keyword(fcs_file, "$PAR");
	fcs_file->seekg((int) fcs_file->tellg() + 4 + 1);
	length = num_chars_till_keyword(fcs_file, delimiter);
	fcs_file->read(buf, length);
	*num_parameters = atoi(buf);
	memset(buf, 0, sizeof buf);

	//read endianess
	seek_to_keyword(fcs_file, "$BYTEORD");
	fcs_file->seekg((int) fcs_file->tellg() + 8 + 1);
	length = num_chars_till_keyword(fcs_file, delimiter);
	fcs_file->read(buf, length);
	if (strstr(buf, "1,2,3,4") != NULL) {
		*small_endianess = true;
	} else {
		*small_endianess = false;
	}
	memset(buf, 0, sizeof buf);

}

int FcsProcessor::seek_to_keyword(std::ifstream* fcs_file,
		std::string keyword) {
	// Initialize reading buffer and return value
	char buf[256];
	int R = 1;
	int pos = 0;
	// Go back to the starting position on the file
	fcs_file->seekg(std::ifstream::beg);

	// Keep filling up buffer with data from the file
	while (!fcs_file->eof()) {
		fcs_file->read(buf, sizeof(buf));
		// Check if the buffer contains the desired keyword
		char * off = strstr(buf, keyword.c_str());
		if (off != NULL) {
			// If it does, set the cursor to the beginning of the keyword
			pos += off - buf;
			fcs_file->seekg(pos);
			break;
		}
		pos += 256;
	}

	// Return
	return R;
}

int FcsProcessor::num_chars_till_keyword(std::ifstream* fcs_file,
		std::string keyword) {
	char buf[256];
	int R = 0;

	int current_pos = fcs_file->tellg();

	// Keep filling up buffer with data from the file
	while (!fcs_file->eof()) {
		fcs_file->read(buf, sizeof(buf));
		// Check if the buffer contains the desired keyword
		char * off = strstr(buf, keyword.c_str());
		if (off != NULL) {
			// If it does, set the cursor to the beginning of the keyword
			R += off - buf;
			break;
		}
		R += 256;
	}

	fcs_file->seekg(current_pos);
	// Return
	return R;
}

void FcsProcessor::read_parameters(std::ifstream* fcs_file,
		bool small_endianness, size_t num_parameters, float * parameters) {
	// Initialize the read buffer
	char buf[4];
	char buf_reordered[4];

	// Initialize a float
	float g;

	// Loop for the number of floats to read
	for (size_t i = 0; i < num_parameters; i++) {
		// Fill up the buffer
		fcs_file->read(buf, 4);

		if (!small_endianness) {
			// Swap the bytes to change the endianness
			buf_reordered[0] = buf[3];
			buf_reordered[1] = buf[2];
			buf_reordered[2] = buf[1];
			buf_reordered[3] = buf[0];

			// Copy the byte array into the float
			g = *((float*) buf_reordered);
		} else {
			g = *((float*) buf);
		}

		// Place the received float into the output array
		parameters[i] = g;
	}

}


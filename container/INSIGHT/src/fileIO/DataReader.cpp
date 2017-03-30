/*
 * DataReader.cpp
 *
 *  Created on: Sep 27, 2012
 *      Author: jan
 *
 *      This class is used to read the data from the file into the data matrix.
 */

#include "DataReader.h"

#include <fstream>
#include <string>

#include "FcsProcessor.h"
#include "IllegalArgumentException.h"

namespace INSIGHTv3 {

DataReader::DataReader() {

}

DataReader::~DataReader() {
}

void DataReader::readDataOneFile(std::string data_file_path, int num_timepoints,
		int num_measured_species, int num_samples,
		std::vector<EiVector>* original_data) {

	std::ifstream data_file(data_file_path.c_str());

	for (int num_vectors = 0;
			num_vectors < num_timepoints * num_measured_species;
			num_vectors++) {
		original_data->push_back(EiVector(num_samples));

	}
	if (data_file.is_open()) {
		for (int sample_nbr = 0; sample_nbr < num_samples; sample_nbr++) {
			for (int time_pt_nbr = 0; time_pt_nbr < num_timepoints;
					time_pt_nbr++) {
				for (int species_nbr = 0; species_nbr < num_measured_species;
						species_nbr++) {
					double sample;
					data_file >> sample;
					((*original_data)[time_pt_nbr + species_nbr])(sample_nbr) =
							sample;
				}
			}
		}
	} else {
		std::ostringstream os;
		os << "Data file " << data_file_path << " could not be read!"
				<< std::endl;
		throw IllegalArgumentException(os.str().c_str());
	}
}

EiVector DataReader::readFCSFile(std::string data_file, int sample_size,
		int fcs_parameter) {
	EiVector vector(sample_size);

	std::ifstream fcs_file;
	fcs_file.open((const char*) data_file.c_str(), std::ifstream::in);
	if (!fcs_file.is_open()) {
		std::ostringstream os;
		os << "Data file " << data_file << " could not be read!"
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

	// Iterate through the parameters that have to be read
	// Position the file cursor at the beginning of the data segment
	fcs_file.seekg(dos);

	// Read the first M events greater than 1.1 of each parameter
	for (int event = 0; event < sample_size; event++) {
		// Get a chunk of floats
		do {
			FcsProcessor::read_parameters(&fcs_file, small_endianess, par,
					fbuf);
		} while (fbuf[fcs_parameter - 1] <= 1.1);

		// Place the data in the proper spot
		vector(event) = fbuf[fcs_parameter - 1];
	}

	fcs_file.close();

	return vector;
}

EiVector DataReader::readTextFile(std::string data_file_path, int sample_size) {
	std::ifstream data_file(data_file_path.c_str());

	EiVector vector(sample_size);

	if (data_file.is_open()) {
		for (int sample_nbr = 0; sample_nbr < sample_size; sample_nbr++) {
			double sample;
			data_file >> sample;
			vector(sample_size) = sample;
		}
	} else {
		std::ostringstream os;
		os << "Data file " << data_file_path << " could not be read!"
				<< std::endl;
		throw IllegalArgumentException(os.str().c_str());
	}
	return vector;
}

} /* namespace INSIGHTv3 */

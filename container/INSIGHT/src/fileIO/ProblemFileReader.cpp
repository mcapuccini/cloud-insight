/*
 * ProblemFileReader.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: jan
 *
 *      This class reads the information from the provided problem.xml files.
 */

#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

#include "ModelFactory.h"
#include "ProblemFileReader.h"
#include "IllegalArgumentException.h"
#include "BGReader.h"
#include "DataReader.h"
#include "IOUtils.h"

namespace INSIGHTv3 {

ProblemFileReader::ProblemFileReader() {
	_model = NULL;
	_original_data = new std::vector<EiVector>();
	_lower_bounds = NULL;
	_upper_bounds = NULL;
	_times = new std::vector<double>();

}

ProblemFileReader::~ProblemFileReader() {
	// TODO Auto-generated destructor stub
}

ModelDescription ProblemFileReader::read(RngPtr r, std::string problem_file) {

	bt_ptree pt;
	boost::property_tree::read_xml(problem_file, pt);

	std::string problem_file_path = problem_file.substr(0,
			problem_file.find_last_of("/") +1);

	_createModel(r, pt, problem_file_path);
	_createPriors(pt);
	_createData(pt, problem_file_path);

	return ModelDescription(_model, _original_data, _lower_bounds,
			_upper_bounds, _times);
}

void ProblemFileReader::_createModel(RngPtr r, bt_ptree pt,
		std::string problem_file_path) {
	MODELS model_nbr = (MODELS) pt.get<int>("problem.model.model_code");
	bool use_bg_fl = ModelFactory::modelRequiresBg(model_nbr);

	//read measured species
	std::vector<int> measured_species;
	BOOST_FOREACH(bt_ptree::value_type & v, pt.get_child("problem.model.measuredspecies")){
	measured_species.push_back(v.second.get<int>("index"));
}

// The background fluorescence is assumed to be in fcs format. If this is not justified an extension needs to be implemented
	if (use_bg_fl) {

		std::vector<EiVector> bg_measured_species;

		int test_index = pt.get("problem.model.background.measurement.index",
				-1); //this is a workaround to make sure, that the background node exists. TODO A better solution should be found
		if (test_index == -1) {
			std::cout
					<< "Background fluourescence required by model, but no background fluourescence file is provided. Background fluorescence is assumed to be zero."
					<< std::endl;
			for (size_t species_nbr = 0; species_nbr < measured_species.size();
					species_nbr++) {
				EiVector bg(1);
				bg << 0.0;
				bg_measured_species.push_back(bg);
			}
		} else {
			std::vector<std::string> bg_paths;
			std::vector<size_t> fcs_parameters;
			std::vector<size_t> sample_sizes;

			BOOST_FOREACH(bt_ptree::value_type & v, pt.get_child("problem.model.background")){
			std::string bg_file = v.second.get<std::string>("fcsfile");

			std::string bg_file_absolute = IOUtils::getAbsolutePath(bg_file,
					problem_file_path);
			bg_paths.push_back(bg_file_absolute);
			fcs_parameters.push_back(v.second.get<size_t>("fcsparam"));
			sample_sizes.push_back(v.second.get<size_t>("samples"));
		}

			if (bg_paths.size() != fcs_parameters.size()) {
				throw new IllegalArgumentException(
						"Not enough fcs prameter numbers provided for each background fluorescence file!");
			}

			for (size_t bg_file_nbr = 0; bg_file_nbr < bg_paths.size();
					bg_file_nbr++) {
				bg_measured_species.push_back(
						BGReader::readSingleBGFile(bg_paths[bg_file_nbr],
								sample_sizes[bg_file_nbr],
								fcs_parameters[bg_file_nbr]));
			}
		}

		_model = ModelFactory::getModelWithFl(model_nbr, r, measured_species,
				bg_measured_species);
	} else {

		_model = ModelFactory::getModel(model_nbr, r, measured_species);
	}

}

void ProblemFileReader::_createPriors(bt_ptree pt) {

	int current_parameter = 0;
	_lower_bounds = new EiVector(_model->num_params);
	_upper_bounds = new EiVector(_model->num_params);

	BOOST_FOREACH(bt_ptree::value_type & v, pt.get_child("problem.model.priors")){
	int current_index = v.second.get<int>("index");
	if(current_parameter != current_index) {
		throw new IllegalArgumentException("the indexing of the prior does not seem to be consistent");
	}
	(*_lower_bounds)(current_parameter) = v.second.get<double>("lb");
	(*_upper_bounds)(current_parameter) = v.second.get<double>("ub");
	current_parameter++;
}
}

void ProblemFileReader::_createData(bt_ptree pt,
		std::string problem_file_path) {

	bool fcs_mode = pt.get<std::string>("problem.data.<xmlattr>.mode").compare(
			"fcs") == 0;
	bool one_file =
			pt.get<std::string>("problem.data.<xmlattr>.one_file").compare(
					"yes") == 0;

	//So far it is assumed, that if only one file is used, this file is a regular text file where the data for all measured species and all timepoints is saved.
	if (one_file) {
		int sample_size = pt.get<int>("problem.data.samples");
		std::string data_file = pt.get<std::string>("problem.data.datafile");
		std::string data_file_absolute = IOUtils::getAbsolutePath(data_file,
				problem_file_path);

		int num_timepoints;

		BOOST_FOREACH(bt_ptree::value_type & v, pt.get_child("problem.data")){
		if(v.first.compare("timept") == 0) {
			_times->push_back(v.second.get<double>("time"));
		}

	}
		num_timepoints = _times->size();

		DataReader::readDataOneFile(data_file_absolute, num_timepoints,
				_model->num_outputs, sample_size, _original_data);

	} else {
		if (fcs_mode) {

			BOOST_FOREACH(bt_ptree::value_type & v, pt.get_child("problem.data")){
			if(v.first.compare("timept") == 0) {
				int sample_size = v.second.get<int>("samples");
				std::string data_file = v.second.get<std::string>("fcsfile");
				std::string data_file_absolute = IOUtils::getAbsolutePath(data_file, problem_file_path);
				_times->push_back(v.second.get<double>("time"));

				BOOST_FOREACH(bt_ptree::value_type & measured_species, v.second.get_child("measuredspecies")) {
					int fcs_parameter = measured_species.second.get<int>("fcsparam");
					_original_data->push_back( DataReader::readFCSFile(data_file_absolute, sample_size, fcs_parameter));
				}
			}
		}
	} else {
		BOOST_FOREACH(bt_ptree::value_type & v, pt.get_child("problem.data")) {
			int sample_size = v.second.get<int>("samples");
			_times->push_back(v.second.get<double>("time"));

			BOOST_FOREACH(bt_ptree::value_type & measured_species, v.second.get_child("measuredspecies")) {
				std::string data_file = measured_species.second.get<std::string>("datafile");
				std::string data_file_absolute = IOUtils::getAbsolutePath(data_file, problem_file_path);
				_original_data->push_back( DataReader::readTextFile(data_file_absolute, sample_size));
			}
		}
	}

}

}

}
/* namespace INSIGHTv3 */

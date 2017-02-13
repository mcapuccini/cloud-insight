/*
 * IOUtils.cpp
 *
 *  Created on: Dec 13, 2012
 *      Author: jan
 *
 *      Utility class to modify file names
 */

#include "IOUtils.h"
#include <string>

namespace INSIGHTv3 {

IOUtils::IOUtils() {
	// TODO Auto-generated constructor stub

}

IOUtils::~IOUtils() {
	// TODO Auto-generated destructor stub
}

std::string IOUtils::appendToFileName(const std::string fileName,
		const std::string string) {
	size_t pos = fileName.find_last_of(".");
	std::string fileStart;
	std::string fileEnd;
	std::string file;
	fileStart.assign(fileName.begin(), fileName.begin() + pos);
	fileEnd.assign(fileName.begin() + pos, fileName.end());
	file = fileStart + "_" + string + fileEnd;
	return file;
}

std::string IOUtils::getAbsolutePath(std::string path, std::string base_path){
	if(path.find_first_of("/") == 0){
		return path;
	}else{
		return base_path + path;
	}
}

std::string IOUtils::getFileLocation(std::string full_file_path){
	size_t end_of_path = full_file_path.find_last_of("/");
	std::string file_path;
	file_path.assign(full_file_path.begin(), full_file_path.begin() + end_of_path);
	return file_path;
}

} /* namespace INSIGHTv3 */

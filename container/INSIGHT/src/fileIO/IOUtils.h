/*
 * IOUtils.h
 *
 *  Created on: Dec 13, 2012
 *      Author: jan
 */

#ifndef IOUTILS_H_
#define IOUTILS_H_

#include <string>

namespace INSIGHTv3 {

class IOUtils {
public:
	IOUtils();
	virtual ~IOUtils();

	static std::string appendToFileName(const std::string fileName,
			const std::string string);
	static std::string getAbsolutePath(std::string path, std::string base_path);
	static std::string getFileLocation(std::string full_file_path);
};

} /* namespace INSIGHTv3 */

#endif /* IOUTILS_H_ */

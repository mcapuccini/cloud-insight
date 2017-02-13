/*
 * DimensionMissmatchException.h
 *
 *  Created on: Jan 24, 2014
 *      Author: jan
 */

#ifndef DIMENSIONMISSMATCHEXCEPTION_H_
#define DIMENSIONMISSMATCHEXCEPTION_H_

#include <exception>

#include <string>

namespace INSIGHTv3 {

class DimensionMissmatchException: public std::exception {
public:
	DimensionMissmatchException(const char* msg) :
			_err_msg(msg) {
	}
	;
	virtual ~DimensionMissmatchException() throw () {
	}
	;
	const char* what() const throw () {
		return ("Dimension missmatch! " + this->_err_msg).c_str();
	}

private:
	std::string _err_msg;
};

} /* namespace INSIGHTv3 */

#endif /* DIMENSIONMISSMATCHEXCEPTION_H_ */

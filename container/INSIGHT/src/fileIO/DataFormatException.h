/*
 * DataFormatException.h
 *
 *  Created on: Sep 28, 2012
 *      Author: jan
 */

#ifndef DATAFORMATEXCEPTION_H_
#define DATAFORMATEXCEPTION_H_

#include <string>

#include <exception>

class DataFormatException: public std::exception {
public:
	DataFormatException(const char* msg) :
	_err_msg(msg) {
	}
	;
	virtual ~DataFormatException() throw () {
	}
	;

	const char* what() const throw () {
		return ("Data format not supported. " + this->_err_msg).c_str();
	}

private:
	std::string _err_msg;
};

#endif /* DATAFORMATEXCEPTION_H_ */

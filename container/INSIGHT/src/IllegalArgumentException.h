/*
 * IllegalArgumentException.h
 *
 *  Created on: Jan 31, 2014
 *      Author: jan
 */

#ifndef ILLEGALARGUMENTEXCEPTION_H_
#define ILLEGALARGUMENTEXCEPTION_H_

#include <exception>
#include <string>

namespace INSIGHTv3 {

class IllegalArgumentException: public std::exception {
public:
	IllegalArgumentException(const char* msg) :
			_err_msg(msg) {
	}
	;
	virtual ~IllegalArgumentException() throw () {
	}
	;

	const char* what() const throw () {
		return ("Illegal argument! " + this->_err_msg).c_str();
	}

private:
	std::string _err_msg;
};

} /* namespace INSIGHTv3 */

#endif /* ILLEGALARGUMENTEXCEPTION_H_ */

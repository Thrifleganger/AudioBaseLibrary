/*
 * AudioException.h
 *
 *  Created on: Feb 22, 2017
 *      Author: Thrifleganger
 */

#ifndef AUDIOEXCEPTION_H_
#define AUDIOEXCEPTION_H_

#include <iostream>

class AudioException {

public:
	enum ExceptionType{
		UNKNOWN_EXCEPTION = 0,
		ENV_VALUES_NEGATIVE,
		ENV_VALUES_BAD
	};

	virtual void processException(ExceptionType type);
};



#endif /* AUDIOEXCEPTION_H_ */

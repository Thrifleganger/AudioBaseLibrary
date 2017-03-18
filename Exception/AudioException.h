/*
 * AudioException.h
 *
 *  Created on: Feb 22, 2017
 *      Author: Thrifleganger
 */

#ifndef AUDIOEXCEPTION_H_
#define AUDIOEXCEPTION_H_

#include <iostream>
#include <string>

#define DEBUG_INFO __FILE__,__LINE__,__func__

enum ERROR_TYPE{
		NO_ERROR = 0,
		SIZE_MISMATCH,
		ZERO_VALUE,
		OPEN_FILE_TO_WRITE,
		UNEXPECTED_CHANNELS
	};


class AudioException {
protected:
	ERROR_TYPE errorNumber;
	std::string errorMessage;
	int lineNumber;
	std::string functionName;
	std::string fileName;

public:

	void printErrorToConsole() {

		std::cout << fileName << " : " << lineNumber << " " << functionName << "() : ";
		switch(errorNumber){
		case NO_ERROR:
			std::cout << "No error.\n";
			break;
		case SIZE_MISMATCH:
			std::cout << "Size mismatch error. " << errorMessage << std::endl;
			break;
		case ZERO_VALUE:
			std::cout << "Value cannot be zero. " << errorMessage << std::endl;
			break;
		case OPEN_FILE_TO_WRITE:
			std::cout << "Could not open file to write: " << errorMessage << std::endl;
			break;
		case UNEXPECTED_CHANNELS:
			std::cout << "Unexpected number of audio channels encountered. " << errorMessage << std::endl;
			break;
		}
	}

	void printErrorToConsole(ERROR_TYPE number) {
		errorNumber = number;
		printErrorToConsole();
	}

	void setErrorNumber(ERROR_TYPE number) { errorNumber = number; }

	void setErrorMessage(std::string message) { errorMessage = message; }

	void setErrorMessage(const char *message) { errorMessage = message; }

	ERROR_TYPE getErrorNumber() { return errorNumber; }

	const char* getErrorMessage() { return errorMessage.c_str(); }

	void setError(ERROR_TYPE number, std::string message, std::string file, int line, std::string func) {
		errorNumber = number;
		errorMessage = message;
		fileName = file;
		lineNumber = line;
		functionName = func;
	}

	void setError(ERROR_TYPE number, std::string message) {
		errorNumber = number;
		errorMessage = message;
	}
};



#endif /* AUDIOEXCEPTION_H_ */

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

/**
 * Macro definition to be used with setError() function. This macro is used as a convenience over
 * manually setting file name, line number and function name values.
 */
#define DEBUG_INFO __FILE__,__LINE__,__func__

enum ERROR_TYPE{
		NO_ERROR = 0,
		UNDEFINED_ERROR,
		SIZE_MISMATCH,
		ZERO_VALUE,
		OPEN_FILE_TO_READ,
		OPEN_FILE_TO_WRITE,
		SEEK_BEYOND_FILE,
		UNEXPECTED_CHANNELS
	};

/**
 * Class for logging error information. This class can be used for conveniently logging error messages to
 * the console.
 * @todo Add flexibilty by logging to file.
 */
class AudioException {
protected:
	ERROR_TYPE errorNumber;
	std::string errorMessage;
	int lineNumber;
	std::string functionName;
	std::string fileName;

public:

	/**
	 * Constructor for AudioException class. Called automatically when inherited. Need not be explicitly called.
	 */
	AudioException() : errorNumber(NO_ERROR), errorMessage(""), lineNumber(0), functionName(""), fileName("") {}
	/**
	 * Prints a set error message to the console. setError() method should be called prior calling this method.
	 * Prints an error description to `standard error`.
	 */
	void printErrorToConsole() {

		if(!functionName.empty() && !fileName.empty() && lineNumber != 0)
			std::cerr << fileName << " : " << lineNumber << " " << functionName << "() : ";
		switch(errorNumber){
		case NO_ERROR:
			std::cerr << "No error.\n";
			break;
		case UNDEFINED_ERROR:
			std::cerr << errorMessage << std::endl;
			break;
		case SIZE_MISMATCH:
			std::cerr << "Size mismatch error. " << errorMessage << std::endl;
			break;
		case ZERO_VALUE:
			std::cerr << "Value cannot be zero. " << errorMessage << std::endl;
			break;
		case OPEN_FILE_TO_READ:
			std::cerr << "Could not open file to read: " << errorMessage << std::endl;
			break;
		case OPEN_FILE_TO_WRITE:
			std::cerr << "Could not open file to write: " << errorMessage << std::endl;
			break;
		case SEEK_BEYOND_FILE:
			std::cerr << "Could not seek to position. Position beyond the length of the file. " << errorMessage << std::endl;
			break;
		case UNEXPECTED_CHANNELS:
			std::cerr << "Unexpected number of audio channels encountered. " << errorMessage << std::endl;
			break;
		}
	}

	/**
	 * Quickly print a known error number or retrieved error number to the console. This method should be called
	 * ideally by retrieving an error flag from getErrorNumber() from the target function.
	 */
	void printErrorToConsole(ERROR_TYPE number) {
		errorNumber = number;
		printErrorToConsole();
	}

	/**
	 * Sets error number.
	 * @param number The error number which is an ERROR_TYPE enum
	 */
	void setErrorNumber(ERROR_TYPE number) { errorNumber = number; }

	/**
	 * Sets error message.
	 * @param message Message of C++ type std::string
	 */
	void setErrorMessage(std::string message) { errorMessage = message; }

	/**
	 * Sets error message.
	 * @param message Message of constant character array.
	 */
	void setErrorMessage(const char *message) { errorMessage = message; }

	/**
	 * Retrieve error number.
	 * @return Error number which is an ERROR_TYPE enum.
	 */
	ERROR_TYPE getErrorNumber() { return errorNumber; }

	/**
	 * Retrieve error message.
	 * @return Error message of constant character array.
	 */
	const char* getErrorMessage() { return errorMessage.c_str(); }

	/**
	 * Sets error parameters. This method is called before printing values to the console.
	 * Choose an error number from the ERROR_TYPE enum. If a particular error is not listed in the enum,
	 * choose UNDEFINED_ERROR.
	 * > Use the macro `DEBUG INFO` for convenience, replacing the last 3 parameters with this single macro.0
	 * Usage example:
	 * @code setError(UNDEFINED_ERROR, "This is an undefined error", DEBUG_INFO); @endcode
	 * @param number Choose error numer from the ERROR_TYPE enum.
	 * @param message Describe the error in detail. A generic message is already included depending on the error number chosen
	 * @param file File name of the propogated error. Use __FILE__ macro. Or skip entirely with DEBUG_INFO macro.
	 * @param line Line number of the propogated error. Use __LINE__ macro. Or skip entirely with DEBUG_INFO macro.
	 * @param func Function name of the propogated error. Use __func__ macro. Or skip entirely with DEBUG_INFO macro.
	 *
	 */
	void setError(ERROR_TYPE number, std::string message, std::string file, int line, std::string func) {
		errorNumber = number;
		errorMessage = message;
		fileName = file;
		lineNumber = line;
		functionName = func;
	}

	/**
	 * Sets error parameters without additional debug information. This method is called before printing values
	 * to the console. Choose an error number from the ERROR_TYPE enum. If a particular error is not listed in the
	 * enum, choose UNDEFINED_ERROR. This method does not log additional information such as file name, line number,
	 * and function name of the propogated error.
	 * @param number Choose error numer from the ERROR_TYPE enum.
	 * @param message Describe the error in detail. A generic message is already included depending on the error number chosen
	 */
	void setError(ERROR_TYPE number, std::string message) {
		errorNumber = number;
		errorMessage = message;
	}
};



#endif /* AUDIOEXCEPTION_H_ */

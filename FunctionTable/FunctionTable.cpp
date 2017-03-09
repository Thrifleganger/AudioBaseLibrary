/*
 * FunctionTable.cpp
 *
 *  Created on: Mar 8, 2017
 *      Author: Thrifleganger
 */

#include "FunctionTable.h"
#include <vector>
#include <cmath>

FuncTable::FuncTable(unsigned int s, const double *tab, bool norm) {
	size = s;
	table = new double[size + 2];
	normalize = norm;
	if(tab){
		memcpy(table, tab, size * sizeof(double));
		//Wrap around points for linear and cubic interpolation.
		table[size+1] = table[1];
		table[size] = table[0];
	} else {
		for(unsigned int i = 0; i < size; i++){
			table[i] = 0.0;
		}
	}
	if(normalize)
		normalizeTable();
}

void FuncTable::normalizeTable() {
	double maxSize = 0.0;
	for(unsigned int i = 0; i < size; i++)
		maxSize = fabs(table[i]) > maxSize ? fabs(table[i]) : maxSize;
	if(maxSize) {
		for(unsigned int i = 0; i < size; i++)
			table[i] /= maxSize;
	}
}

FourierTable::FourierTable(const unsigned int harmonics, const double *ampArray,
			const double phase, const unsigned int size): FuncTable(size) {
	createTable(harmonics, ampArray, phase);
}

FourierTable::FourierTable(const unsigned int harmonics, const unsigned int type,
			const double phase, const unsigned int size) : FuncTable(size) {

	std::vector<double> partialStrength(harmonics, 0.0);

	switch(type) {
	case(SINE) :
			partialStrength[0] = 1;
			break;
	case(SAWTOOTH) :
			for(unsigned int i = 0; i < harmonics; i++)
				partialStrength[i] = 1.0 / (double)(i+1);
			break;
	case(TRIANGLE) :
			for(unsigned int i = 0; i < harmonics; i+=2)
				partialStrength[i] = 1.0 / (double)((i+1) * (i+1));
			break;
	case(SQUARE) :
			for(unsigned int i = 0; i < harmonics; i+=2)
				partialStrength[i] = 1.0 / (double)(i+1);
			break;
	default:
			for(unsigned int i = 0; i < harmonics; i++)
				partialStrength[i] = 1.0;
			break;
	}

	createTable(harmonics, partialStrength.data(), phase);
}

void FourierTable::createTable(const unsigned int harmonics, const double *ampArray,
			const double phase) {

	double amp;
	for(unsigned int partial = 0; partial < harmonics; partial++) {
		for(unsigned int i = 0; i < size; i++) {
			amp = ampArray != NULL ? ampArray[partial] : 1.0;
			table[i] += amp * sin(((partial + 1) * i * TWOPI / size) + phase);
		}
	}
}

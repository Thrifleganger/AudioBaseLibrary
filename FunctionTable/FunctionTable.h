/*
 * FunctionTable.h
 *
 *  Created on: Mar 8, 2017
 *      Author: Thrifleganger
 */

#ifndef FUNCTIONTABLE_H_
#define FUNCTIONTABLE_H_

#include "AudioBase.h"

enum wave_type {
	SINE = 1,
	SAWTOOTH,
	TRIANGLE,
	SQUARE
};

class FuncTable : public AudioParams{
protected:
	double *table;
	unsigned int size;
	bool normalize;

	void normalizeTable();

public:
	FuncTable(unsigned int size = def_tsize, const double *tab = NULL, bool norm = false);

	~FuncTable(){ delete[] table; }

	double *getTable() const { return table; }

	unsigned int getSize() const{ return size; }
};

class SinTable : public FuncTable {
public:
	SinTable(unsigned int s = def_tsize) : FuncTable(s) {
		for(unsigned int i = 0; i < size; i++)
			table[i] = sin(i*TWOPI/size);
	}
};

class FourierTable : public FuncTable {
protected:
	void createTable(const unsigned int harmonics, const double *ampArray,
			const double phase);

public:
	FourierTable(const unsigned int harmonics = 1, const double *ampArray = NULL,
			const double phase = 0., const unsigned int size = def_tsize);

	FourierTable(const unsigned int harmonics, const unsigned int type,
			const double phase = 0., const unsigned int size = def_tsize);

};


#endif /* FUNCTIONTABLE_H_ */

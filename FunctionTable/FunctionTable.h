/*
 * FunctionTable.h
 *
 *  Created on: Mar 8, 2017
 *      Author: Thrifleganger
 */

#ifndef FUNCTIONTABLE_H_
#define FUNCTIONTABLE_H_

#include "AudioBase.h"
#include <vector>
#include "AudioException.h"

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

class SampleTable : public AudioException{
protected:
	std::vector<double> sampTab;
	int channels;
	double samplerate;
	long frames;
	AudioException exception;

public:
	SampleTable() : channels(0), samplerate(0), frames(0) {}
	SampleTable(const char* fileName);

	int getChannels() { return channels; }
	double getSampleRate() { return samplerate; }
	long getFrames() { return frames; }
	std::vector<double> getSampleTable() { return sampTab; }

	const double operator[](int index) { return sampTab[index];	}
};

class SampleReader : public AudioBuffer {
protected:
	double frameCount;
	double skipTime;
	bool wrapAround;
	SampleTable sampleTable;
public:

	SampleReader(const SampleTable &sampTable, double skipTime = 0.0, bool wrapAround = false);

	const AudioBuffer &process(double playbackSpeed = 1.0);

};

#endif /* FUNCTIONTABLE_H_ */

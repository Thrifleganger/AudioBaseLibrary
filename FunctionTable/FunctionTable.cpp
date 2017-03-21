/*
 * FunctionTable.cpp
 *
 *  Created on: Mar 8, 2017
 *      Author: Thrifleganger
 */

#include "FunctionTable.h"
#include <sndfile.h>
#include <vector>
#include <cmath>
#include "AudioException.h"
#include <cstdlib>
#include <exception>

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

SampleTable::SampleTable(const char* fileName) {
	int bufferSize = 1024;
	long framesRead = 0;
	double *readBuffer = new double[bufferSize];

	SF_INFO info;
	SNDFILE *openFile = sf_open(fileName ,SFM_READ, &info);
	frames=0; channels =0; samplerate=0;
	if(openFile != NULL) {
		channels = info.channels;
		samplerate = info.samplerate;
		frames = (long) info.frames;
 		sampTab.resize(frames * channels + 1, 0.0);

		int count = 0;
		framesRead = sf_readf_double(openFile, readBuffer, bufferSize / channels);
		while(framesRead != 0) {
			for(int i = 0; i < framesRead; i++)
				sampTab[count++] = readBuffer[i];
			framesRead = sf_readf_double(openFile, readBuffer, bufferSize / channels);
		}
	} else {
		exception.setError(OPEN_FILE_TO_READ, fileName, DEBUG_INFO);
		exception.printErrorToConsole();
		exit(exception.getErrorNumber());
	}
	if(openFile)
		sf_close(openFile);
}

SampleReader::SampleReader(const SampleTable &sampTable, double skipTime, bool wrapAround) {
	this->wrapAround = wrapAround;
	sampleTable = sampTable;
	this->skipTime = (int)skipTime * sampleTable.getSampleRate();
	if(skipTime >= sampleTable.getFrames() || skipTime < 0) {
		exception.setError(SEEK_BEYOND_FILE, "", DEBUG_INFO);
		exception.printErrorToConsole();
		exit(exception.getErrorNumber());
	}
	frameCount = this->skipTime;
};

const AudioBuffer& SampleReader::process(double speed) {
	int posi;
	double frac;
	for(unsigned int i = 0; i < getVectorSize(); i++) {
		posi = (int) frameCount;
		frac = frameCount - posi;
		if(speed >= 0) {
			if(frameCount < sampleTable.getFrames()) {
				vector[i] = sampleTable[posi] + frac * (sampleTable[posi + 1] - sampleTable[posi]);
				frameCount += speed;
			}
			else if(wrapAround)
				frameCount = 0;
			else
				vector[i] = 0;
		} else {
			if(frameCount > 0) {
				vector[i] = sampleTable[posi] + frac * (sampleTable[posi + 1] - sampleTable[posi]);
				frameCount += speed;
			}
			else if(wrapAround)
				frameCount = sampleTable.getFrames();
			else
				vector[i] = 0;
		}
	}

	return *this;
}

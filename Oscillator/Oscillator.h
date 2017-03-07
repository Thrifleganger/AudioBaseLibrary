/*
 * Oscillator.h
 *
 *  Created on: Feb 7, 2017
 *      Author: Thrifleganger
 */

#ifndef OSCILLATOR_H_
#define OSCILLATOR_H_

#include <cmath>
#include <cstring>
#include "AudioBase.h"

const double _TWO_PI = 8. * atan(1.);

class FuncTable : public AudioParams{
protected:
	double *table;
	int size;
public:
	FuncTable(int s, const double *tab = NULL){
		size = s;
		table = new double[size + 2];
		if(tab){
			memcpy(table, tab, size * sizeof(double));
			//Wrap around points for linear and cubic interpolation.
			table[size+1] = table[1];
			table[size] = table[0];
		}
	}
	~FuncTable(){
		delete[] table;
	}

	double *getTable() const{
		return table;
	}

	int getSize() const{
		return size;
	}
};

class SinTable : public FuncTable {
public:
	SinTable(int s) : FuncTable(s) {
		for(int i=0; i < size; i++)
			table[i] = sin(i*_TWO_PI/size);
	}
};


class Oscil : public AudioParams{

protected:

	double amplitude;
	double frequency;
	double *table;
	int size;
	double phase;
	double *buffer;
	void updatePhase();

public:

	virtual void oscillator();

	Oscil(double a, double f, const FuncTable &t, double phs = 0.) :
			amplitude(a), frequency(f), table(t.getTable()), size(t.getSize()),
			phase(phs), buffer(new double[getVectorSize()]) {
	}

	virtual ~Oscil() {
		delete[] buffer;
	}

	const double *process() {
		oscillator();
		return buffer;
	}
	const double *process(double a, double f) {
		amplitude = a;
		frequency = f;
		oscillator();
		return buffer;
	}
	const double *process(double a) {
		amplitude = a;
		oscillator();
		return buffer;
	}

};

class Oscili: public Oscil {
public:
	Oscili(double a,double f, const FuncTable &tab,
			double phs = 0.) :
	Oscil(a,f,tab,phs) {}
	void oscillator();
	// overrides Osc::oscillator()
};

class Oscilc: public Oscil {
public:
	Oscilc(double a,double f, const FuncTable &tab,
			double phs = 0.) :
	Oscil(a,f,tab,phs) {}
	void oscillator();
	// overrides Osc::oscillator()
};

class Phasor: public Oscil {
public:
	Phasor(double a, double f, const FuncTable &tab,
			double phs = 0.) :
	Oscil(a,f,tab,phs) {}
	void oscillator();
};

class TableReader : public AudioParams {

protected:
	double *refTable;
	int size;
	bool normalized;
	bool wrap;
	double *buffer;

public:
	TableReader(FuncTable &tab, bool norm = true, bool wrap = true):
	refTable(tab.getTable()), size(tab.getSize()), normalized(norm),
	wrap(wrap), buffer() {};

	double *readTable(const double *phaseTab);

};

class WhiteNoise : public AudioParams {

protected:
	double amplitude;
	double *buffer;
	virtual void generate();

public:
	WhiteNoise(double amplitude) : amplitude(amplitude),
		buffer(new double[getVectorSize()]) {}

	virtual ~WhiteNoise() {
		delete[] buffer;
	}

	const double *process() {
		generate();
		return buffer;
	}
};



#endif /* OSCILLATOR_H_ */

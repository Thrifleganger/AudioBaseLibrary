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
#include "FunctionTable.h"

class Oscil : public AudioBuffer{

protected:

	const static SinTable sinTab;
	double amplitude;
	double frequency;
	double *table;
	int size;
	double phase;
	const double *ampMod;
	const double *freqMod;

	void updatePhase();
	void checkModulation(int index);

public:

	virtual void oscillator();

	Oscil(double a, double f, const FuncTable &t = sinTab, double phs = 0.) :
			amplitude(a), frequency(f), table(t.getTable()), size(t.getSize()),
			phase(phs), ampMod(NULL), freqMod(NULL) {
	}

	virtual ~Oscil() {
	}

	//Change
	virtual const Oscil &process() {
		oscillator();
		return *this;
	}

	virtual const Oscil &process(double a, double f) {
		amplitude = a;
		frequency = f;
		oscillator();
		return *this;
	}

	virtual const Oscil &process(double a) {
		amplitude = a;
		oscillator();
		return *this;
	}

	virtual const Oscil &process(const AudioBuffer &obja) {
		ampMod = obja.getVector();
		oscillator();
		return *this;
	}

	virtual const Oscil &process(const AudioBuffer &obja, float f) {
		ampMod = obja.getVector();
		frequency = f;
		oscillator();
		return *this;
	}

	virtual const Oscil &process(const AudioBuffer &obja,const AudioBuffer &objf) {
		ampMod = obja.getVector();
		freqMod = objf.getVector();
		oscillator();
		return *this;
	}

	virtual const Oscil &process(float a,const AudioBuffer &objf) {
		amplitude = a;
		freqMod = objf.getVector();
		oscillator();
		return *this;
	}

	virtual const Oscil &operator()() { return process(); }

	virtual const Oscil &operator()(double a) { return process(a); }

	virtual const Oscil &operator()(double a, double f) { return process(a, f); }

	virtual const Oscil &operator()(const AudioBuffer &obja) { return process(obja); }

	virtual const Oscil &operator()(const AudioBuffer &obja, double f) { return process(obja, f); }

	virtual const Oscil &operator()(const AudioBuffer &obja,const AudioBuffer &objf) { return process(obja, objf); }

	virtual const Oscil &operator()(double a,const AudioBuffer &objf) { return process(a, objf); }


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

//****************************************MODIFY
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

class WhiteNoise : public AudioBuffer {

protected:
	double amplitude;
	const double *ampMod;

	virtual void generate();
	virtual void checkModulation(int index);

public:
	WhiteNoise(const double amplitude = 0.5) : amplitude(amplitude), ampMod(NULL) {}

	virtual ~WhiteNoise() {}

	const AudioBuffer &process() {
		generate();
		return *this;
	}

	const AudioBuffer &process(const double amp) {
		amplitude = amp;
		generate();
		return *this;
	}

	const AudioBuffer &process(const AudioBuffer &buffer) {
		ampMod = buffer.getVector();
		generate();
		return *this;
	}

	const AudioBuffer &operator()() { return process(); }

	const AudioBuffer &operator()(const double a) { return process(a); }

	const AudioBuffer &operator()(const AudioBuffer &b) { return process(b); }
};



#endif /* OSCILLATOR_H_ */

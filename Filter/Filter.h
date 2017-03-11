/*
 * Filter.h
 *
 *  Created on: Feb 22, 2017
 *      Author: Thrifleganger
 */

#ifndef FILTER_H_
#define FILTER_H_

#include "AudioBase.h"

class FirstOrderFilter : public AudioBuffer {

protected:
	double coeffA;
	double coeffB;
	double rc;
	double cutOff;
	double delSig;
	const double *signal;
	const double *cutOffMod;

	virtual void filter() = 0;
	virtual void update() = 0;
	virtual void checkModulation(int index);

public:
	FirstOrderFilter(double cutOff) : coeffA(0.0), coeffB(0.0), rc(0.0), cutOff(cutOff), delSig(0.0),
		signal(NULL), cutOffMod(NULL) {}

	virtual ~FirstOrderFilter() {}

	virtual const AudioBuffer &process(const AudioBuffer &signal, double cutOffFrequency);

	virtual const AudioBuffer &process(const AudioBuffer &signal, const AudioBuffer &cutOffFrequency);

	virtual const AudioBuffer &operator()(const AudioBuffer &sig, double cutOff) {
		return process(sig, cutOff);	}

	virtual const AudioBuffer &operator()(const AudioBuffer &sig, const AudioBuffer &cutOff) {
		return process(sig, cutOff); 	}
};

class ToneLP : public FirstOrderFilter {

protected:
	void update();
	void filter();

public:
	ToneLP(double cutOff) : FirstOrderFilter(cutOff) {}
};


class ToneHP : public FirstOrderFilter {

protected:
	void update();
	void filter();

public:
	ToneHP() :FirstOrderFilter(0) {}
	ToneHP(double cutOff) : FirstOrderFilter(cutOff) {}
};


class SecondOrderFilter : public AudioBuffer {

protected:
	double *coeffA;
	double *coeffB;
	double L;
	double M;
	double w;
	double y;
	double cutOff;
	double bandwidth;
	double *delSig;
	const double *signal;
	const double *cutOffMod;
	const double *bwMod;

	virtual void filter();
	virtual void update() = 0;
	virtual void checkModulation(int index);

public:
	SecondOrderFilter() : coeffA(new double[10]), coeffB(new double[10]), L(0), M(0), w(0), y(0),
		cutOff(0), bandwidth(0), delSig(new double[10]), signal(NULL), cutOffMod(NULL), bwMod(NULL) { }

	SecondOrderFilter(double co, double bw) : coeffA(new double[10]), coeffB(new double[10]), L(0), M(0), w(0), y(0),
		cutOff(co), bandwidth(bw), delSig(new double[10]), signal(NULL), cutOffMod(NULL), bwMod(NULL) { }

	virtual ~SecondOrderFilter() {
		delete[] coeffA;
		delete[] coeffB;
		delete[] delSig;
	}
};

class Butterworth : public SecondOrderFilter {
protected:
	virtual void update() = 0;

public:
	Butterworth() : SecondOrderFilter() { }

	virtual ~Butterworth() {}

	virtual const AudioBuffer &process(const AudioBuffer &signal, double cutOffFrequency);

	virtual const AudioBuffer &process(const AudioBuffer &signal, const AudioBuffer &cutOffFrequency);

	virtual const AudioBuffer &operator()(const AudioBuffer &signal, double cutOffFrequency) {
		return process(signal, cutOffFrequency); }

	virtual const AudioBuffer &operator()(const AudioBuffer &signal, const AudioBuffer &cutOffFrequency) {
			return process(signal, cutOffFrequency); }
};

class ButterLP : public Butterworth {
protected:
	void update();

public:
	ButterLP();
};

class ButterHP : public Butterworth {
protected:
	void update();

public:
	ButterHP();
};


class ButterworthBand : public SecondOrderFilter {
protected:
	virtual void update() = 0;

public:
	ButterworthBand() : SecondOrderFilter() { }
	ButterworthBand(double cutOff, double bw) : SecondOrderFilter(cutOff, bw) {}

	virtual ~ButterworthBand() {}

	virtual const AudioBuffer &process(const AudioBuffer &signal, double cutOffFrequency,
			double bandwidth);

	virtual const AudioBuffer &process(const AudioBuffer &signal, const AudioBuffer &cutOffFrequency,
			double bandwidth);

	virtual const AudioBuffer &process(const AudioBuffer &signal, double cutOffFrequency,
			const AudioBuffer &bandwidth);

	virtual const AudioBuffer &process(const AudioBuffer &signal, const AudioBuffer &cutOffFrequency,
			const AudioBuffer &bandwidth);

	virtual const AudioBuffer &operator()(const AudioBuffer &signal, double cutOffFrequency,
			double bandwidth) {	return process(signal, cutOffFrequency, bandwidth); }

	virtual const AudioBuffer &operator()(const AudioBuffer &signal, const AudioBuffer &cutOffFrequency,
			double bandwidth) {	return process(signal, cutOffFrequency, bandwidth); }

	virtual const AudioBuffer &operator()(const AudioBuffer &signal, double cutOffFrequency,
			const AudioBuffer &bandwidth) {	return process(signal, cutOffFrequency, bandwidth); }

	virtual const AudioBuffer &operator()(const AudioBuffer &signal, const AudioBuffer &cutOffFrequency,
			const AudioBuffer &bandwidth) {	return process(signal, cutOffFrequency, bandwidth); }
};


class ButterBP : public ButterworthBand {
protected:
	void update();

public:
	ButterBP();
	ButterBP(double cutOff, double bw) : ButterworthBand(cutOff, bw) {}
};

class ButterBR : public ButterworthBand {
protected:
	void update();

public:
	ButterBR();
	ButterBR(double cutOff, double bw) : ButterworthBand(cutOff, bw) {}
};


#endif /* FILTER_H_ */

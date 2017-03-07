/*
 * Filter.h
 *
 *  Created on: Feb 22, 2017
 *      Author: Thrifleganger
 */

#ifndef FILTER_H_
#define FILTER_H_

#include "AudioBase.h"

class FirstOrderFilter : public AudioParams {

protected:
	double coeffA;
	double coeffB;
	double rc;
	double cutOff;
	double delSig;
	const double *signal;
	double *buffer;

	virtual void filter() = 0;

	virtual void update() = 0;

public:
	FirstOrderFilter() : coeffA(0.0), coeffB(0.0), rc(0.0), cutOff(0.0), delSig(0.0),
		signal(new double[getVectorSize()]), buffer(new double[getVectorSize()]) {}

	virtual ~FirstOrderFilter() {
		delete[] buffer;
		delete[] signal;
	}

	virtual double *process(const double *signal, double cutOffFrequency);
};

class ToneLP : public FirstOrderFilter {

protected:
	void update();
	void filter();

public:
	ToneLP() {}
};


class ToneHP : public FirstOrderFilter {

protected:
	void update();
	void filter();

public:
	ToneHP() {}
};


class Butterworth : public AudioParams {

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
	double *buffer;

	virtual void filter();
	virtual void update() = 0;

public:
	Butterworth() : coeffA(new double[10]), coeffB(new double[10]), L(0), M(0), w(0), y(0),
		cutOff(0), bandwidth(0), delSig(new double[10]), signal(new double[getVectorSize()]),
		buffer(new double[getVectorSize()]) { }

	virtual ~Butterworth() {
		delete[] coeffA;
		delete[] coeffB;
		delete[] delSig;
		delete[] signal;
		delete[] buffer;
	}

	virtual double *process(const double *signal, double cutOffFrequency, double bandWidth);
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

class ButterBP : public Butterworth {
protected:
	void update();

public:
	ButterBP();
};

class ButterBR : public Butterworth {
protected:
	void update();

public:
	ButterBR();
};


#endif /* FILTER_H_ */

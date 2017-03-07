/*
 * Filter.cpp
 *
 *  Created on: Feb 22, 2017
 *      Author: Thrifleganger
 */
#include "Filter.h"
#include <cmath>

double *FirstOrderFilter::process(const double *sig, double cutOffFrequency) {
	signal = sig;
	if(cutOff != cutOffFrequency) {
		cutOff = cutOffFrequency;
		update();
	}
	else
		filter();
	return buffer;
}

// ************* Divide by zero error check
void ToneLP::update() {
	if(getSrate() != 0) {
		rc = 2 - cos(TWOPI * cutOff / getSrate());
		coeffB = sqrt(rc*rc - 1) - rc;
		coeffA = 1 + coeffB;
	}
}

void ToneLP::filter() {
	for(unsigned int i = 0; i < getVectorSize(); i++) {
		delSig = coeffA * signal[i] - coeffB * delSig;
		buffer[i] = delSig;
	}
}



// ************* Divide by zero error check
void ToneHP::update() {
	if(getSrate() != 0) {
		rc = 2 + cos(TWOPI * cutOff / getSrate());
		coeffB = rc - sqrt(rc*rc - 1);
		coeffA = 1 + coeffB;
	}
}

void ToneHP::filter() {
	for(unsigned int i = 0; i < getVectorSize(); i++) {
		delSig = coeffA * signal[i] - coeffB * delSig;
		buffer[i] = delSig;
	}
}



double *Butterworth::process(const double *sig, double cutOffFrequency, double BW) {
	signal = sig;
	if(cutOff != cutOffFrequency || bandwidth != BW) {
		cutOff = cutOffFrequency;
		bandwidth = BW;
		update();
	}
	else
		filter();
	return buffer;
}

void Butterworth::filter() {
	for(unsigned int i = 0; i < getSrate(); i++) {
		w = signal[i] -  (coeffB[0] * delSig[0]) - (coeffB[1] * delSig[1]);
		y = (coeffA[0] * w) + (coeffA[1] * delSig[0]) + (coeffA[2] * delSig[1]);

		delSig[1] = delSig[0];
		buffer[i] = delSig[0] = w;
	}
}

void ButterLP::update() {

	L = pow(tan(PI * cutOff / getSrate()), -1);

	coeffA[0] = pow(1 + sqrt(2) * L + L*L, -1);
	coeffA[1] = 2 * coeffA[0];
	coeffA[2] = coeffA[0];
	coeffB[0] = 2 * (1 - L*L) * coeffA[0];
	coeffB[1] = (1 - sqrt(2) * L + L*L) * coeffA[0];
}

void ButterHP::update() {

	L = pow(tan(PI * cutOff / getSrate()), -1);

	coeffA[0] = pow(1 + sqrt(2) * L + L*L, -1);
	coeffA[1] = -2 * coeffA[0];
	coeffA[2] = coeffA[0];
	coeffB[0] = 2 * (L*L - 1) * coeffA[0];
	coeffB[1] = (1 - sqrt(2) * L + L*L) * coeffA[0];
}

void ButterBP::update() {

	M = pow(tan(PI * bandwidth / getSrate()), -1);

	coeffA[0] = pow(1 + M, -1);
	coeffA[1] = 0;
	coeffA[2] = -coeffA[0];
	coeffB[0] = -2 * M * cos(TWOPI * cutOff / getSrate()) * coeffA[0];
	coeffB[1] = (M - 1) * coeffA[0];
}

void ButterBR::update() {

	M = pow(tan(PI * bandwidth / getSrate()), -1);

	coeffA[0] = pow(1 + M, -1);
	coeffA[1] = -2 * cos(TWOPI * cutOff / getSrate()) * coeffA[0];
	coeffA[2] = coeffA[0];
	coeffB[0] = coeffA[1];
	coeffB[1] = (1 - M) * coeffA[0];
}

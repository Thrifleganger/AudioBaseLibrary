/*
 * Filter.cpp
 *
 *  Created on: Feb 22, 2017
 *      Author: Thrifleganger
 */
#include "Filter.h"
#include <cmath>

const AudioBuffer &FirstOrderFilter::process(const AudioBuffer &sig, double cutOffFrequency) {
	signal = sig.getVector();
	if(cutOff != cutOffFrequency) {
		cutOff = cutOffFrequency;
		update();
	}
	else
		filter();
	return *this;
}

const AudioBuffer &FirstOrderFilter::process(const AudioBuffer &sig, const AudioBuffer &cutOffFrequency) {
	signal = sig.getVector();
	cutOffMod = cutOffFrequency.getVector();
	cutOff = cutOffMod[0];
	update();
	filter();
	return *this;
}

void FirstOrderFilter::checkModulation(int index) {
	if(cutOffMod != NULL) {
		cutOff = cutOffMod[index];
		update();
	}
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
		checkModulation(i);
		delSig = coeffA * signal[i] - coeffB * delSig;
		vector[i] = delSig;
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
		checkModulation(i);
		delSig = coeffA * signal[i] - coeffB * delSig;
		vector[i] = delSig;
	}
}


//Second order filter:

void SecondOrderFilter::filter() {
	for(unsigned int i = 0; i < getVectorSize(); i++) {
		checkModulation(i);
		w = signal[i] -  (coeffB[0] * delSig[0]) - (coeffB[1] * delSig[1]);
		y = (coeffA[0] * w) + (coeffA[1] * delSig[0]) + (coeffA[2] * delSig[1]);

		delSig[1] = delSig[0];
		vector[i] = delSig[0] = w;
	}
}

void SecondOrderFilter::checkModulation(int index) {
	if(cutOffMod != NULL || bwMod != NULL) {
		cutOff = cutOffMod != NULL ? cutOffMod[index] : cutOff;
		bandwidth = bwMod != NULL ? bwMod[index] : bandwidth;
		update();
	}
}


const AudioBuffer &Butterworth::process(const AudioBuffer &sig, double cutOffFrequency) {
	signal = sig.getVector();
	if(cutOff != cutOffFrequency) {
		cutOff = cutOffFrequency;
		update();
	}
	else
		filter();
	return *this;
}

const AudioBuffer &Butterworth::process(const AudioBuffer &sig, const AudioBuffer &cutOffFrequency) {
	signal = sig.getVector();
	cutOffMod = cutOffFrequency.getVector();
	cutOff = cutOffMod[0];
	update();
	filter();
	return *this;
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



//////////////////////

const AudioBuffer &ButterworthBand::process(const AudioBuffer &sig, double co, double bw) {
	signal = sig.getVector();
	if(cutOff != co || bandwidth != bw) {
		cutOff = co;
		bandwidth = bw;
		update();
	}
	else
		filter();
	return *this;
}

const AudioBuffer &ButterworthBand::process(const AudioBuffer &sig, const AudioBuffer &co, double bw) {
	signal = sig.getVector();
	cutOffMod = co.getVector();
	cutOff = cutOffMod[0];
	bandwidth = bw;
	update();
	filter();
	return *this;
}

const AudioBuffer &ButterworthBand::process(const AudioBuffer &sig, double co, const AudioBuffer &bw) {
	signal = sig.getVector();
	cutOff = co;
	bwMod = bw.getVector();
	bandwidth = bwMod[0];
	update();
	filter();
	return *this;
}

const AudioBuffer &ButterworthBand::process(const AudioBuffer &sig, const AudioBuffer &co, const AudioBuffer &bw) {
	signal = sig.getVector();
	cutOffMod = co.getVector();
	bwMod = bw.getVector();
	cutOff = cutOffMod[0];
	bandwidth = bwMod[0];
	update();
	filter();
	return *this;
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

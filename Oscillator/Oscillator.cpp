/*
 * Oscillator.cpp
 *
 *  Created on: Feb 8, 2017
 *      Author: Thrifleganger
 */

#include "Oscillator.h"
#include <cstdlib>

void Oscil::oscillator() {
	for (unsigned int i = 0; i < getVectorSize(); i++) {
		checkModulation(i);
		vector[i] = amplitude * table[(int) phase];
		updatePhase();
	}
}

void Oscili::oscillator() {
	double frac;
	int posi;
	for (unsigned int i = 0; i < getVectorSize(); i++) {
		checkModulation(i);
		posi = (int) phase;
		frac = phase - posi;
		vector[i] = amplitude * (table[posi] + frac * (table[posi + 1] - table[posi]));
		updatePhase();
	}
}

void Oscilc::oscillator() {
	double frac, a, b, c, d;
	double tmp, fracsq, fracb;
	int posi;
	for (unsigned int i = 0; i < getVectorSize(); i++) {
		checkModulation(i);
		posi = (int) phase;
		frac = phase - posi;
		a = posi == 0 ? table[0] : table[posi - 1];
		b = table[posi];
		c = table[posi + 1];
		d = table[posi + 2];
		tmp = d + 3.f * b;
		fracsq = frac * frac;
		fracb = frac * fracsq;
		vector[i] = amplitude
				* (fracb * (-a - 3.f * c + tmp) / 6.f
						+ fracsq * ((a + c) / 2.f - b)
						+ frac * (c + (-2.f * a - tmp) / 6.f) + b);
		updatePhase();
	}
}

inline void Oscil::updatePhase(){
	phase += size * frequency / getSrate();
	while (phase >= size)
		phase -= size;
	while (phase < 0)
		phase += size;
}

void Oscil::checkModulation(int index) {
	if(ampMod != NULL)
		amplitude = ampMod[index];
	if(freqMod != NULL)
		frequency = freqMod[index];
}

void Phasor::oscillator() {
	double increment = frequency / getSrate();
	for (unsigned int i = 0; i < getVectorSize(); i++) {
		vector[i] = phase;
		phase += increment;
		phase = fmod(phase, 1);
	}
}

double* TableReader::readTable(const double *phaseTab){
	int vsize = getVectorSize();
	buffer = new double[vsize];

	for(int i = 0; i < vsize; i++) {
		buffer[i] = refTable[(int)(phaseTab[i] * size)];
	}

	return buffer;

}

void WhiteNoise::generate() {
	for(unsigned int i = 0; i < getVectorSize(); i++) {
		buffer[i] = amplitude * ((rand() % 2001 / 1000.0) - 1);
	}
}

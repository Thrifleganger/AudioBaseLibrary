/*
 * Delay.cpp
 *
 *  Created on: Mar 11, 2017
 *      Author: Thrifleganger
 */
#include "AudioBase.h"
#include "Delay.h"
#include <cmath>

void Delay::setSampleWise(double &number) {
	switch(metric) {
	case SECONDS:
		number = number * getSrate();
		break;
	case MILLIS:
		number = number * getSrate() / 1000.0;
		break;
	case SAMPS:
	default:
		break;
	}
}

void Delay::checkModulation(int index) {
	if(delayTimeVector != NULL) {
		delayTime = delayTimeVector[index];
		setSampleWise(delayTime);
		delayTime = delayTime <= bufferSize ?
				(delayTime < 0 ? 0 : delayTime) : bufferSize;
	}
	if(feedbackVector != NULL)
		feedback = feedbackVector[index];
}

/*void Delay::dsp() {
	for(unsigned int i = 0; i < getVectorSize(); i++) {
		checkModulation(i);
		vector[i] = delayBuffer[rwPosition];
		delayBuffer[rwPosition] = signal[i] + delayBuffer[rwPosition] * feedback;
		rwPosition = rwPosition < delayTime ? rwPosition + 1 : 0;
	}
}*/

void Delay::dsp() {
	double x1, x2, y1, y2, x;
	for(unsigned int i = 0; i < getVectorSize(); i++) {
		checkModulation(i);
		x = writePosition - delayTime;
		if(x < 0)
			x += bufferSize;
		x1 = (int)x;
		y1 = delayBuffer[x1];
		x2 = x1 + 1 < bufferSize ? x1 + 1 : 0;
		y2 = delayBuffer[x2];

		vector[i] = y1 + ((x - x1) * (y2 - y1));
		delayBuffer[writePosition] = signal[i] + vector[i] * feedback;
		writePosition = writePosition < bufferSize ? writePosition + 1 : 0;
	}
}

double Delay::getFeedbackFromDecay(double decayTime) {
	return pow((1/1000.0), (delayTime/getSrate())/decayTime);
}

double Delay::getFeedbackFromDecay(double decayTime, double delTime) {
	return pow((1/1000.0), delTime/decayTime);
}

void Allpass::dsp() {
	//VERIFY *****************************************
	double x1, x2, y1, y2, x, y, node;
	for(unsigned int i = 0; i < getVectorSize(); i++) {
		checkModulation(i);
		x = writePosition - delayTime;
		if(x < 0)
			x += bufferSize;
		x1 = (int)x;
		y1 = delayBuffer[x1];
		x2 = x1 + 1 < bufferSize ? x1 + 1 : 0;
		y2 = delayBuffer[x2];

		y = y1 + ((x - x1) * (y2 - y1));

		node = signal[i] + delayBuffer[writePosition] * feedback;
		vector[i] = delayBuffer[writePosition] - node * feedback;
		delayBuffer[writePosition] = node;
		writePosition = writePosition < bufferSize ? writePosition + 1 : 0;
	}
}



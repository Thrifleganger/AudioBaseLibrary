/*
 * Delay.h
 *
 *  Created on: Mar 11, 2017
 *      Author: Thrifleganger
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "AudioBase.h"
#include <vector>

enum DELAY_TIME_METRIC {
	SECONDS = 0,
	MILLIS,
	SAMPS
};

class Delay : public AudioBuffer {
protected:
	double delayTime;
	double bufferSize;
	DELAY_TIME_METRIC metric;
	std::vector<double> delayBuffer;
	const double *signal;
	int writePosition;
	int readPosition;
	int rwPosition;
	double feedback;

	const double *delayTimeVector;
	const double *feedbackVector;

	virtual void setSampleWise(double &size);
	virtual void checkModulation(int index);

	virtual void dsp();

public:

	Delay(const double bufferSize, const double feedback = 0.0, const DELAY_TIME_METRIC metric = SECONDS) :
		delayTime(0.0), bufferSize(bufferSize), metric(metric), delayBuffer(1,0.0),
		signal(NULL), writePosition(0), readPosition(0), rwPosition(0), feedback(feedback),
		delayTimeVector(NULL), feedbackVector(NULL) {

		setSampleWise(this->bufferSize);
		this->bufferSize = (int)this->bufferSize;
		delayTime = this->bufferSize;
		delayBuffer.resize(this->bufferSize + 1, 0.0);
	}

	virtual ~Delay() {}

	virtual const AudioBuffer &process(const AudioBuffer &signal) {
		this->signal = signal.getVector();
		delayTime = bufferSize;
		dsp();
		return *this;
	}

	virtual const AudioBuffer &process(const AudioBuffer &signal, double delayTime) {
		this->signal = signal.getVector();
		this->delayTime = delayTime;
		setSampleWise(this->delayTime);
		this->delayTime = this->delayTime <= bufferSize ?
				(this->delayTime < 0 ? 0 : this->delayTime) : bufferSize;
		dsp();
		return *this;
	}

	virtual const AudioBuffer &process(const AudioBuffer &signal, const AudioBuffer &delayTime) {
		this->signal = signal.getVector();
		delayTimeVector = delayTime.getVector();
		dsp();
		return *this;
	}

	virtual const AudioBuffer &process(const AudioBuffer &signal, double delayTime, double feedback) {
		this->signal = signal.getVector();
		this->delayTime = delayTime;
		setSampleWise(this->delayTime);
		this->delayTime = this->delayTime <= bufferSize ?
				(this->delayTime < 0 ? 0 : this->delayTime) : bufferSize;
		this->feedback = feedback;
		dsp();
		return *this;
	}

	virtual const AudioBuffer &process(const AudioBuffer &signal, const AudioBuffer &delayTime, double feedback) {
		this->signal = signal.getVector();
		this->delayTimeVector = delayTime.getVector();
		this->feedback = feedback;
		dsp();
		return *this;
	}

	virtual const AudioBuffer &process(const AudioBuffer &signal, double delayTime, const AudioBuffer &feedback) {
		this->signal = signal.getVector();
		this->delayTime = delayTime;
		setSampleWise(this->delayTime);
		this->delayTime = this->delayTime <= bufferSize ?
				(this->delayTime < 0 ? 0 : this->delayTime) : bufferSize;
		this->feedbackVector = feedback.getVector();
		dsp();
		return *this;
	}

	virtual const AudioBuffer &process(const AudioBuffer &signal, const AudioBuffer &delayTime, const AudioBuffer &feedback) {
		this->signal = signal.getVector();
		this->delayTimeVector = delayTime.getVector();
		this->feedbackVector = feedback.getVector();
		dsp();
		return *this;
	}

	virtual const AudioBuffer &operator()(const AudioBuffer &signal) {
		return process(signal); }

	virtual const AudioBuffer &operator()(const AudioBuffer &signal, double delayTime) {
		return process(signal, delayTime); }

	virtual const AudioBuffer &operator()(const AudioBuffer &signal, const AudioBuffer &delayTime) {
		return process(signal, delayTime); }

	virtual const AudioBuffer &operator()(const AudioBuffer &signal, double delayTime, double feedback) {
		return process(signal, delayTime, feedback); }

	virtual const AudioBuffer &operator()(const AudioBuffer &signal, const AudioBuffer &delayTime, double feedback) {
		return process(signal, delayTime, feedback); }

	virtual const AudioBuffer &operator()(const AudioBuffer &signal, double delayTime, const AudioBuffer &feedback) {
		return process(signal, delayTime, feedback); }

	virtual const AudioBuffer &operator()(const AudioBuffer &signal, const AudioBuffer &delayTime, const AudioBuffer &feedback) {
		return process(signal, delayTime, feedback); }

	double getFeedbackFromDecay(double decayTime);

	double getFeedbackFromDecay(double decayTime, double delayTime);

	double getDelayTime() const {
		return delayTime;
	}

	void setDelayTime(double delayTime) {
		this->delayTime = delayTime;
	}

	double getFeedback() const {
		return feedback;
	}

	void setFeedback(double feedback) {
		this->feedback = feedback;
	}
};


class Allpass : public Delay {

protected:
	void dsp();

public:
	Allpass(const double delayTime, const double feedback = 0.0, const DELAY_TIME_METRIC metric = SECONDS) :
		Delay(delayTime, feedback, metric) {}
};

#endif /* DELAY_H_ */

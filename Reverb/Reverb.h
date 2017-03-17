/*
 * Reverb.h
 *
 *  Created on: Mar 16, 2017
 *      Author: Thrifleganger
 */

#ifndef REVERB_H_
#define REVERB_H_

#include "delay.h"

class SReverb : public Delay {

	double decayTime;
	Delay delay1;
	Delay delay2;
	Delay delay3;
	Delay delay4;
	Allpass apass1;
	Allpass apass2;

public:
	SReverb(double decay = 3) : Delay(1.0),
		decayTime(decay),
		delay1(0.0297),
		delay2(0.0371),
		delay3(0.0437),
		delay4(0.0437), apass1(0.005), apass2(0.0017) {
		delay1.setFeedback(delay1.getFeedbackFromDecay(decayTime));
		delay2.setFeedback(delay2.getFeedbackFromDecay(decayTime));
		delay3.setFeedback(delay3.getFeedbackFromDecay(decayTime));
		delay4.setFeedback(delay4.getFeedbackFromDecay(decayTime));
		apass1.setFeedback(apass1.getFeedbackFromDecay(0.0968));
		apass2.setFeedback(apass2.getFeedbackFromDecay(0.0329));
	}

	const AudioBuffer &process(const AudioBuffer &signal) {
		AudioBuffer parallel;
		delay1(signal);
		delay2(signal);
		delay3(signal);
		delay4(signal);
		parallel = delay1 + delay2 + delay3 + delay4;
		apass1(parallel);
		apass2(apass1);
		return apass2;
	}

	const AudioBuffer &operator()(const AudioBuffer &signal) { return process(signal); }
};



#endif /* REVERB_H_ */

/*Author - Thrifleganger
 *
 *AudioBase: base class for all audio applications. Must call AudioBase constructor before audio processing begins.
 */

#ifndef AUDIOBASE_H_
#define AUDIOBASE_H_

#include <cmath>
#include <cstring>
#include <iostream>


//Default variable:

//Vector size for signals (block size/ksmps/control buffer)
const unsigned int def_vsize = 64;
//Sample rate
const unsigned int def_samplerate = 44100;
//Control rate
const unsigned int def_controlrate = def_samplerate / def_vsize;
//Number of channels
const unsigned int def_nchannels = 1;
//Table size
const unsigned int def_tsize = 4096;
//IO buffer size
const unsigned int def_bsize = 1024;
const double PI = 4*atan(1.);
const double TWOPI = 8*atan(1.);

enum {
	AUDIO_REALTIME = 1,
	AUDIO_STDOUT,
	AUDIO_SNDFILE
};

class AudioParams {
protected:
	static unsigned int srate;
	static unsigned int nchannels;
	static unsigned int vectorSize;
	static unsigned int bufferSize;

public:
	static unsigned int getBufferSize() {
		return bufferSize;
	}

	static unsigned int getNchannels() {
		return nchannels;
	}

	static unsigned int getSrate() {
		return srate;
	}

	static unsigned int getVectorSize() {
		return vectorSize;
	}
};

class AudioBase : public AudioParams {
private:
	const char* destination;
	unsigned int mode;
	unsigned int count;
	int frameCount;
	double *vector;
	double *buffer;
	void *handle;

public:

	AudioBase(const char* dest,
			unsigned int nchnls = def_nchannels,
			unsigned int srate = def_samplerate,
			unsigned int vsize = def_vsize,
			unsigned int bsize = def_bsize);

	~AudioBase();

	int write(double* signal);

	void printCurrentState();

	//Getter and setters:
};

class AudioBuffer : public AudioParams {
	double *buffer;
public:
	AudioBuffer(){
		buffer = new double[getVectorSize()];
	}

	~AudioBuffer(){
		delete[] buffer;
	}
};

/*double* operator*(double* array1, double* array2){
	double *tempArray = new double[64];
	for(int i = 0; i < 64; i++) {
		tempArray[i] = array1[i] * array2[i];
	}
	return tempArray;
}*/


#endif /* AUDIOBASE_H_ */

/*Author - Thrifleganger
 *
 *AudioBase: base class for all audio applications. Must call AudioBase constructor before audio processing begins.
 */

#ifndef AUDIOBASE_H_
#define AUDIOBASE_H_

#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>
#include <ctime>


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

class AudioBuffer : public AudioParams {
protected:
	std::vector<double> vector;
public:
	AudioBuffer() : vector(AudioParams::vectorSize * AudioParams::nchannels, 0.0) {}

	virtual ~AudioBuffer(){
		vector.clear();
	}

// + Operator overload

	friend AudioBuffer operator+(const AudioBuffer &obj, double scalar) {
		AudioBuffer temp;
		for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
			temp.vector[i] = obj.vector[i] + scalar;
		return temp;
	}

	friend AudioBuffer operator+(double scalar, const AudioBuffer &obj) {
		AudioBuffer temp;
		for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
			temp.vector[i] = obj.vector[i] + scalar;
		return temp;
	}

	virtual AudioBuffer operator+(const double *array) {
		AudioBuffer temp;
		for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
			temp.vector[i] = vector[i] + array[i];
		return temp;
	}

	virtual AudioBuffer operator+(const std::vector<double> vect) {
		AudioBuffer temp;
		if(this->vectorSize == vect.size()) {
			for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
				temp.vector[i] = vector[i] + vect[i];
		}
		return temp;
	}

	virtual AudioBuffer operator+(const AudioBuffer &obj) {
		return *this + obj.vector;
	}

// += Operator Overload

	virtual AudioBuffer &operator+=(const double scalar) {
		for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
			vector[i] += scalar;
		return *this;
	}

	virtual AudioBuffer &operator+=(const double *array) {
		for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
			vector[i] += array[i];
		return *this;
	}

	virtual AudioBuffer &operator+=(const std::vector<double> vect) {
		if(this->vectorSize == vect.size()) {
			for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
				vector[i] += vect[i];
		}
		return *this;
	}

	virtual AudioBuffer operator+=(const AudioBuffer &obj) {
		return *this += obj.vector;
	}

// - Operator overload

	virtual AudioBuffer operator-(const double scalar) const {
		AudioBuffer temp;
		for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
			temp.vector[i] = vector[i] - scalar;
		return temp;
	}

	virtual AudioBuffer operator-(const double *array) {
		AudioBuffer temp;
		for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
			temp.vector[i] = vector[i] - array[i];
		return temp;
	}

	virtual AudioBuffer operator-(const std::vector<double> vect) {
		AudioBuffer temp;
		if(this->vectorSize == vect.size()) {
			for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
				temp.vector[i] = vector[i] - vect[i];
		}
		return temp;
	}

	virtual AudioBuffer operator-(const AudioBuffer &obj) {
		return *this - obj.vector;
	}

// -= Operator Overload

	virtual AudioBuffer &operator-=(const double scalar) {
		for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
			vector[i] -= scalar;
		return *this;
	}

	virtual AudioBuffer &operator-=(const double *array) {
		for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
			vector[i] -= array[i];
		return *this;
	}

	virtual AudioBuffer &operator-=(const std::vector<double> vect) {
		if(this->vectorSize == vect.size()) {
			for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
				vector[i] -= vect[i];
		}
		return *this;
	}

	virtual AudioBuffer operator-=(const AudioBuffer &obj) {
		return *this -= obj.vector;
	}

// * Operator Overload

	friend AudioBuffer operator*(const AudioBuffer &obj, double scalar) {
		AudioBuffer temp;
		for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
			temp.vector[i] = obj.vector[i] * scalar;
		return temp;
	}

	friend AudioBuffer operator*(double scalar, const AudioBuffer &obj) {
		AudioBuffer temp;
		for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
			temp.vector[i] = obj.vector[i] * scalar;
		return temp;
	}

	virtual AudioBuffer operator*(const double *array) {
		AudioBuffer temp;
		for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
			temp.vector[i] = vector[i] * array[i];
		return temp;
	}

	virtual AudioBuffer operator*(const std::vector<double> vect) {
		AudioBuffer temp;
		if(this->vectorSize == vect.size()) {
			for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
				temp.vector[i] = vector[i] * vect[i];
		}
		return temp;
	}

	virtual AudioBuffer operator*(const AudioBuffer &obj) {
		return *this * obj.vector;
	}

// *= Operator Overload

	virtual AudioBuffer &operator*=(double scalar) {
		for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
			vector[i] *= scalar;
		return *this;
	}

	virtual AudioBuffer &operator*=(const double *array) {
		for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
			vector[i] *= array[i];
		return *this;
	}

	virtual AudioBuffer &operator*=(const std::vector<double> vect) {
		if(this->vectorSize == vect.size()) {
			for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
				vector[i] *= vect[i];
		}
		return *this;
	}

	virtual AudioBuffer &operator*=(const AudioBuffer &obj) {
		return *this *= obj.vector;
	}

// / Operator Overload

	virtual AudioBuffer operator/(double scalar) const {
		AudioBuffer temp;
		if(scalar != 0) {
			for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
				temp.vector[i] = vector[i] / scalar;
		} else {}
			//****************************************Handle error
		return temp;
	}

	virtual AudioBuffer operator/(const double *array) {
		AudioBuffer temp;
		for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
			if(array[i] != 0)
				temp.vector[i] = vector[i] / array[i];
		return temp;
	}

	virtual AudioBuffer operator/(const std::vector<double> vect) {
		AudioBuffer temp;
		if(this->vectorSize == vect.size()) {
			for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
				if(vect[i] != 0)
					temp.vector[i] = vector[i] / vect[i];
		}
		return temp;
	}

	virtual AudioBuffer operator/(const AudioBuffer &obj) {
		return *this / obj.vector;
	}

// /= Operator Overload

	virtual AudioBuffer &operator/=(double scalar) {
		if(scalar != 0) {
			for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
				vector[i] /= scalar;
		}
		return *this;
	}

	virtual AudioBuffer &operator/=(const double *array) {
		for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
			if(array[i] != 0)
				vector[i] /= array[i];
		return *this;
	}

	virtual AudioBuffer &operator/=(const std::vector<double> vect) {
		if(this->vectorSize == vect.size()) {
			for(unsigned int i = 0; i < AudioParams::vectorSize; i++)
				if(vect[i] != 0)
					vector[i] /= vect[i];
		}
		return *this;
	}

	virtual AudioBuffer &operator/=(const AudioBuffer &obj) {
		return *this /= obj.vector;
	}

	const double *getVector() const { return &vector[0]; }

};


class AudioBase : public AudioBuffer {
private:
	const char* destination;
	unsigned int mode;
	unsigned int count;
	int frameCount;
	double *buffer;
	void *handle;
	FILE *textFile;
	clock_t startTime;

protected:
	int errorCode;

public:

	AudioBase(const char* dest,
			unsigned int nchnls = def_nchannels,
			unsigned int srate = def_samplerate,
			unsigned int vsize = def_vsize,
			unsigned int bsize = def_bsize);

	~AudioBase();

	int write(const double* signal);

	int write(const AudioBuffer& buffer) { return write(buffer.getVector()); }

	void printCurrentState();

	//Getter and setters:
};


#endif /* AUDIOBASE_H_ */

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
#include "AudioException.h"

/*
 *	Default vector size for signals (block size/ksmps/control buffer)
 */
const unsigned int def_vsize = 64;
/*
 * Default sampling rate.
 */
const unsigned int def_samplerate = 44100;
/*
 * Default control rate. Sample rate / vector size
 */
const unsigned int def_controlrate = def_samplerate / def_vsize;
/*
 * Default number of audio channels
 */
const unsigned int def_nchannels = 1;
/*
 * Default table size. Should bbe a power of two.
 */
const unsigned int def_tsize = 4096;
/*
 * Default IO buffer size
 */
const unsigned int def_bsize = 1024;
/*
 * Value of Pi
 */
const double PI = 4*atan(1.);
/*
 * Value of 2 * Pi
 */
const double TWOPI = 8*atan(1.);

/*
 * Enumeration for setting destination.
 * AUDIO_REALTIME - Writes audio to default output device in real time using portaudio.
 * AUDIO_STDOUT - Writes audio or control sample data to file.
 * AUDIO_SNDFILE - Write audio to sound file using libsndfile.
 */
enum {
	AUDIO_REALTIME = 1,
	AUDIO_STDOUT,
	AUDIO_SNDFILE
};

/*
 * Class for storing basic audio data members. Values are set using AudioBase class constructor.
 * Values set once persist throughout the application since the data members are static.
 */
class AudioParams {
protected:
	static unsigned int srate;
	static unsigned int nchannels;
	static unsigned int vectorSize;
	static unsigned int bufferSize;

public:
	/*
	 * Get IO buffer size.
	 */
	static unsigned int getBufferSize() {
		return bufferSize;
	}

	/*
	 * Get number of audio channels.
	 */
	static unsigned int getNchannels() {
		return nchannels;
	}

	/*
	 * Get sampling rate.
	 */
	static unsigned int getSrate() {
		return srate;
	}

	/*
	 * Get signal vector size.
	 */
	static unsigned int getVectorSize() {
		return vectorSize;
	}
};

/*
 * Class for handling buffered storage of signal or control values. A vector object of `vector size` length
 * is initialized, and is the basic means of communicating between different DSP classes.
 */
class AudioBuffer : public AudioParams, public AudioException {
protected:
	std::vector<double> vector;
	AudioException exception;
public:
	AudioBuffer() : vector(AudioParams::vectorSize, 0.0) {}

	virtual ~AudioBuffer(){
		vector.clear();
	}

	/*
	 * Returns vector as an array of double values.
	 * @return Array of length `vector size`
	 */
	const double *getVector() const { return &vector[0]; }

	/*
	 * Fill the called AudioBuffer object's vector with another AudioBuffer object's vector
	 * @param signal AudioBuffer object containing the source vector.
	 */
	void fillVector(const AudioBuffer &signal) {
		for(unsigned int i = 0; i < getVectorSize(); i++){
			vector[i] = signal[i];
		}
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

	virtual const double operator[](const int index) const {
		return vector[index];
	}

	virtual AudioBuffer &operator/=(const AudioBuffer &obj) {
		return *this /= obj.vector;
	}

};

/*
 * Class for instantiating and initializing the library. An AudioBase object needs to be declared and
 * initialized before any other DSP operation from the library is performed. The initialization ensures that
 * correct values of variables such as channels, sample rate, signal vector size are set and usable by
 * other DSP classes. Multiple objects of this class can be created to target different destinations. But care
 * should be taken to ensure that they all have same constructor parameters other than `destination`. If not,
 * the latest object's values will dictate the application's parameters, since the members are static.
 */
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

	/*
	 * Constructor for the AudioBase class. This constructs the base object required to perform other
	 * DSP operations. This constructor must be called before any other class is instantiated.
	 * @param destination The target destination. Can be any of the follow:
	 * - _dac_ - Targets system's default output device for real time audio stream.
	 * - _stdout_ - Targets a text file in the current directory.
	 * - _filename_ - Targets an existing / creates a new file for writing in the current directory.
	 * @param nchnls Number of audio channels
	 * @param srate Sampling rate
	 * @param vsize Signal vector size
	 * @param bsize IO buffer size
	 */
	AudioBase(const char* destination,
			unsigned int nchnls = def_nchannels,
			unsigned int srate = def_samplerate,
			unsigned int vsize = def_vsize,
			unsigned int bsize = def_bsize);

	~AudioBase();

	/*
	 * Writes an array to the `destination`. Can be single or multichannel. If single channel, the
	 * array is written as mono. If `nchannels` is 2, a stereo output is generated, duplicating
	 * the same array in both channels
	 * @param signal An array pointer to be written. The array size should be atleast `vector size` in length.
	 * @return Number of frames written. Is equal to the number of samples written / number of channels
	 */
	int write(const double* signal);

	/*
	 * Writes an AudioBuffer object to the `destination`. Can be single or multichannel. If single channel, the
	 * object is written as mono. If `nchannels` is 2, a stereo output is generated, duplicating
	 * the same array in both channels
	 * @param buffer An `AudioBuffer` object.
	 * @return Number of frames written. Is equal to the number of samples written / number of channels
	 */
	int write(const AudioBuffer& buffer) { return write(buffer.getVector()); }

	/*
	 * Writes a couple of arrays as stereo to the `destination`. Targeted for `nchannels` = 2. Generates error
	 * for any other channel number.
	 * @param left Left channel array pointer. The array size should be atleaset `vector size` in length.
	 * @param right Right channel array pointer. The array size should be atleast `vector size` in length.
	 * @return Number of frames written. Is equal to the number of samples written / number of channels
	 */
	int writeStereo(const double* left, const double* right);

	/*
	 * Writes a couple of `AudioBuffer` objects as stereo to the `destination`. Targeted for `nchannels` = 2.
	 * Generates error for any other channel number.
	 * @param left Left channel `AudioBuffer` object.
	 * @param right Right channel `AudioBuffer` object.
	 * @return Number of frames written. Is equal to the number of samples written / number of channels
	 */
	int writeStereo(const AudioBuffer& left, const AudioBuffer& right) {
		return writeStereo(left.getVector(), right.getVector());
	}

	/*
	 * Print essential info and state information to the console. For debugging purpose.
	 */
	void printCurrentState();

};


#endif /* AUDIOBASE_H_ */

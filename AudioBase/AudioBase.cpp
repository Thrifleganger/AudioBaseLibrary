/*
 * AudioBase.cpp
 *
 *  Created on: Feb 14, 2017
 *      Author: Thrifleganger
 */

#include "AudioBase.h"
#include <sndfile.h>
#include <ctime>
#include <cstdlib>

unsigned int AudioParams::srate;
unsigned int AudioParams::nchannels;
unsigned int AudioParams::vectorSize;
unsigned int AudioParams::bufferSize;

AudioBase::AudioBase(const char* dest, unsigned int nchnls,
		unsigned int srate,	unsigned int vsize,	unsigned int bsize)  {

	srand(time(NULL));

	destination = dest;
	AudioParams::nchannels = nchnls;
	AudioParams::srate = srate;
	AudioParams::vectorSize = vsize;
	AudioParams::bufferSize = bsize;
	mode = 0;
	count = 0;
	frameCount = 0;
	errorCode = 0;
	textFile = NULL;

	if (strcmp(destination, "dac") == 0) {

		//Fill later
	} else if (strcmp(destination, "stdout") == 0) {
		mode = AUDIO_STDOUT;
		textFile = fopen("dump.txt", "w");
	} else {
		SF_INFO info;
		info.samplerate = AudioParams::getSrate();
		info.channels = AudioParams::getNchannels();
		info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
		buffer = new double[AudioParams::getBufferSize() * AudioParams::getNchannels()];
		SNDFILE *openFile = sf_open(destination ,SFM_WRITE, &info);
		if(openFile != NULL) {
			handle = (void *) openFile;
			mode = AUDIO_SNDFILE;
		}
	}
}

int AudioBase::write(const double *signal){
	unsigned int vsamples = AudioParams::vectorSize * AudioParams::nchannels;

	if (mode == AUDIO_REALTIME && handle != NULL) {
		//Fill later
	} else if (mode == AUDIO_STDOUT) {
		for(unsigned int i = 0; i < vsamples; i++){
			fprintf(textFile, "%f\n", signal[i]);
			count++;
		}
		frameCount += count / nchannels;
	} else if(mode == AUDIO_SNDFILE && handle != NULL) {
		unsigned int bsamples = AudioParams::bufferSize * AudioParams::nchannels;
		for(unsigned int i = 0; i < vsamples; i++) {
			buffer[count++] = signal[i];
			if(count == bsamples) {
				frameCount += sf_writef_double((SNDFILE*) handle, buffer, AudioParams::bufferSize);
				count = 0;
			}
		}
	} else
		return 0;

	return frameCount;
}

AudioBase::~AudioBase(){
  if(mode == AUDIO_REALTIME && handle != NULL) {
	  //Fill later
  } else if(mode == AUDIO_STDOUT) {
	  if(textFile)
		  fclose(textFile);
  } else if (mode == AUDIO_SNDFILE && handle != NULL){
	  if (count != 0) {
		  sf_writef_double((SNDFILE*) handle, buffer, count);
		  count = 0;
	  }
	  sf_close((SNDFILE *) handle);
	  delete[] buffer;
  }
}

void AudioBase::printCurrentState() {
	std::cout << "Destination: " << destination << std::endl;
	std::cout << "Sample rate: " << AudioParams::srate << std::endl;
	std::cout << "Channels: " << AudioParams::nchannels << std::endl;
	std::cout << "Signal buffer size: " << AudioParams::vectorSize << std::endl;
	std::cout << "IO buffer size: " << AudioParams::bufferSize << std::endl;
	std::cout << "Mode: " << mode << std::endl;
}


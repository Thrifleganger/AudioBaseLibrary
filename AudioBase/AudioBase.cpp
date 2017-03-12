/*
 * AudioBase.cpp
 *
 *  Created on: Feb 14, 2017
 *      Author: Thrifleganger
 */

#include "AudioBase.h"
#include <sndfile.h>
#include <portaudio.h>
#include <ctime>
#include <cstdlib>
#include <ctime>

unsigned int AudioParams::srate;
unsigned int AudioParams::nchannels;
unsigned int AudioParams::vectorSize;
unsigned int AudioParams::bufferSize;

AudioBase::AudioBase(const char* dest, unsigned int nchnls,
		unsigned int srate,	unsigned int vsize,	unsigned int bsize)  {

	srand(time(NULL));
	startTime = clock();

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

		PaError err;
		err = Pa_Initialize();
		if (err == paNoError) {
		  PaStreamParameters outparam{0};
		  PaStream *stream;
		  outparam.device = (PaDeviceIndex)Pa_GetDefaultOutputDevice();
		  outparam.channelCount = nchnls;
		  outparam.sampleFormat = paFloat32;
		  outparam.suggestedLatency = (PaTime)(vectorSize / srate);
		  err = Pa_OpenStream(&stream, NULL, &outparam, srate, vectorSize, paNoFlag,
							  NULL, NULL);
		  if (err == paNoError) {
			err = Pa_StartStream(stream);
			if (err == paNoError) {
			  handle = (void *)stream;
			  mode = AUDIO_REALTIME;
			} else {
			  //m_error = AULIB_RTSTREAM_ERROR;
			  vectorSize = 0;
			}
		  } else {
			//m_error = AULIB_RTOPEN_ERROR;
			vectorSize = 0;
		  }
		} else {
		  //m_error = AULIB_RTINIT_ERROR;
		  vectorSize = 0;
		}
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
		PaError err;
		int bsamples = bufferSize * nchannels;
		float *buffer = new float(bufferSize * nchannels);
		for (unsigned int i = 0; i < vectorSize; i++) {
		  buffer[count++] = signal[i];
		  if (count == bsamples) {
			err = Pa_WriteStream((PaStream *)handle, buffer, bufferSize);
			if (err == paNoError) {
			  frameCount += count / nchannels;
			}
			count = 0;
		  }
		}
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
  std::cout << "Execution time (sec): " << (double)(clock() - startTime)/CLOCKS_PER_SEC << std::endl;
}

void AudioBase::printCurrentState() {
	std::cout << "Destination: " << destination << std::endl;
	std::cout << "Sample rate: " << AudioParams::srate << std::endl;
	std::cout << "Channels: " << AudioParams::nchannels << std::endl;
	std::cout << "Signal buffer size: " << AudioParams::vectorSize << std::endl;
	std::cout << "IO buffer size: " << AudioParams::bufferSize << std::endl;
	std::cout << "Mode: " << mode << std::endl;
}


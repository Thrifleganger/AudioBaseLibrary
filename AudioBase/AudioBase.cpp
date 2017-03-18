/*
 * AudioBase.cpp
 *
 *  Created on: Feb 14, 2017
 *      Author: Thrifleganger
 */

#include "AudioBase.h"
#include "AudioException.h"
#include <sndfile.h>
//#include <portaudio.h>
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

	if(nchannels == 0 || vectorSize == 0 || bufferSize == 0) {
		exception.setError(ZERO_VALUE, "Number of channels | Vector size | Buffer size", DEBUG_INFO);
		exception.printErrorToConsole();
		exit(exception.getErrorNumber());
	}

	if(bufferSize < vectorSize) {
		exception.setError(SIZE_MISMATCH, "Buffer size cannot be smaller than the vector size", DEBUG_INFO);
		exception.printErrorToConsole();
		exit(exception.getErrorNumber());
	}

	if (strcmp(destination, "dac") == 0) {

/*		PaError err;
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
*/
	} else if (strcmp(destination, "stdout") == 0) {
		mode = AUDIO_STDOUT;
		textFile = fopen("dump.txt", "w");
		if(textFile == NULL) {
			exception.setError(OPEN_FILE_TO_WRITE, "dump.txt", DEBUG_INFO);
			exception.printErrorToConsole();
			exit(exception.getErrorNumber());
		}
	} else {
		SF_INFO info;
		info.samplerate = AudioParams::getSrate();
		info.channels = AudioParams::getNchannels();
		info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
		buffer = new double[AudioParams::getBufferSize()];
		SNDFILE *openFile = sf_open(destination ,SFM_WRITE, &info);
		if(openFile != NULL) {
			handle = (void *) openFile;
			mode = AUDIO_SNDFILE;
		} else {
			exception.setError(OPEN_FILE_TO_WRITE, destination, DEBUG_INFO);
			exception.printErrorToConsole();
			exit(exception.getErrorNumber());
		}
	}
}

int AudioBase::write(const double *signal){

	if (mode == AUDIO_REALTIME && handle != NULL) {
/*		PaError err;
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
*/
	} else if (mode == AUDIO_STDOUT) {
		for(unsigned int i = 0; i < AudioParams::vectorSize; i++){
			fprintf(textFile, "%f\n", signal[i]);
			count++;
		}
		frameCount += count;
	} else if(mode == AUDIO_SNDFILE && handle != NULL) {
		for(unsigned int i = 0; i < AudioParams::vectorSize; i++) {
			for(unsigned int j = 0; j < AudioParams::nchannels; j++)
				buffer[count++] = signal[i];
			if(count >= AudioParams::bufferSize) {
				frameCount += sf_writef_double((SNDFILE*) handle, buffer, AudioParams::bufferSize/AudioParams::nchannels);
				count = 0;
			}
		}
	} else
		return 0;

	return frameCount;
}

int AudioBase::writeStereo(const double *left, const double *right){

	if(nchannels != 2) {
		exception.setError(UNEXPECTED_CHANNELS, "Number of channels should be 2.", DEBUG_INFO);
		exception.printErrorToConsole();
		exit(exception.getErrorNumber());
	}
	if (mode == AUDIO_REALTIME && handle != NULL) {
/*		PaError err;
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
*/
	} else if (mode == AUDIO_STDOUT) {
		for(unsigned int i = 0; i < AudioParams::vectorSize; i++){
			fprintf(textFile, "%f\t%f\n", left[i], right[i]);
			count++;
		}
		frameCount += count;
	} else if(mode == AUDIO_SNDFILE && handle != NULL) {
		for(unsigned int i = 0; i < AudioParams::vectorSize; i++) {
			buffer[count++] = left[i];
			buffer[count++] = right[i];
			if(count >= AudioBuffer::bufferSize) {
				frameCount += sf_writef_double((SNDFILE*) handle, buffer, AudioParams::bufferSize/AudioParams::nchannels);
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


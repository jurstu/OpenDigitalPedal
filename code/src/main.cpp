#include <iostream>
#include "portaudio.h"
#include <cstring>
#include <cmath>

#define SAMPLE_RATE 192000
#define CHANNELS 2
#define FRAMES_PER_BUFFER 64

// Callback function for the input stream
int inputCallback(const void *inputBuffer, void *outputBuffer,
                   unsigned long framesPerBuffer,
                   const PaStreamCallbackTimeInfo *timeInfo,
                   PaStreamCallbackFlags statusFlags,
                   void *userData) {
    // Just copy the input data to the output
    for (int i = 0; i < framesPerBuffer * CHANNELS; i++)
    {
//	continue;
    	short* a = ((short*)inputBuffer+i);
	float* x;
        *a /=2;


	short cutLevel = 65536/32;
	*a = fmax(-cutLevel, *a);
	*a = fmin( cutLevel ,*a);


	short makeUp = 65536/cutLevel/4;

//	*a *= makeUp;


//	printf("%d\n", *a);
//
	//*x = (*x/(abs(*x)+1)) * (1 - exp((*x) * (*x) / (abs(*x)+1)));
	//*x *= 2048/4;
	
//	printf("%f\n", *x);
//	*x = fmax(-65536/16, *x);

//	*x = fmin(65536/16   , *x);

//	*a = (short) *x;

    }
    memcpy(outputBuffer, inputBuffer, framesPerBuffer * CHANNELS * sizeof(short));
    return paContinue;
}

int main() {
    PaError err;

    err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio initialization failed: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }
int numDevices;
numDevices = Pa_GetDeviceCount();
if( numDevices < 0 )
{
    printf( "ERROR: Pa_CountDevices returned 0x%x\n", numDevices );
    err = numDevices;
    return -1;
}

const   PaDeviceInfo *deviceInfo;
for(int i=0; i<numDevices; i++ )
{
    deviceInfo = Pa_GetDeviceInfo( i );
    std::cout<<deviceInfo->name<<std::endl;
}



    PaStream *stream;
    PaStreamParameters inputParameters, outputParameters;

    inputParameters.device = 1;
    inputParameters.channelCount = CHANNELS;
    inputParameters.sampleFormat = paInt16;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(1)->defaultHighInputLatency;
    inputParameters.hostApiSpecificStreamInfo = nullptr;

    outputParameters.device = 1; // Use the default output device for playback
    outputParameters.channelCount = CHANNELS;
    outputParameters.sampleFormat = paInt16;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultHighOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = nullptr;

    err = Pa_OpenStream(&stream,
                        &inputParameters,
                        &outputParameters,
                        SAMPLE_RATE,
                        FRAMES_PER_BUFFER,
                        paNoFlag,
                        inputCallback,
                        nullptr);


//    err = Pa_OpenDefaultStream(&stream,
//                               CHANNELS,
  //                             CHANNELS,
    //                           paInt16,
      //                         SAMPLE_RATE,
        //                       FRAMES_PER_BUFFER,
          //                     inputCallback,
            //                   nullptr);

    if (err != paNoError) {
        std::cerr << "Error opening stream: " << Pa_GetErrorText(err) << std::endl;
        Pa_Terminate();
        return -1;
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "Error starting stream: " << Pa_GetErrorText(err) << std::endl;
        Pa_CloseStream(stream);
        Pa_Terminate();
        return -1;
    }

    std::cout << "Live playback... (Press Ctrl+C to stop)" << std::endl;

    // Keep the program running until interrupted
    while (true) {
        Pa_Sleep(100);  // Sleep for a short duration to avoid high CPU usage
    }

    // The program will never reach this point due to the infinite loop

    // Stop and close the stream
    err = Pa_StopStream(stream);
    if (err != paNoError) {
        std::cerr << "Error stopping stream: " << Pa_GetErrorText(err) << std::endl;
    }

    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        std::cerr << "Error closing stream: " << Pa_GetErrorText(err) << std::endl;
    }

    Pa_Terminate();

    return 0;
}

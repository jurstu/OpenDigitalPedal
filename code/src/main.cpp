#include <iostream>
#include "portaudio.h"
#include <cstring>
#include <cmath>
#include <vector>
#include "../inc/filter.h"
#include <stdint.h>

#define SAMPLE_RATE 192000
#define CHANNELS 1 
#define FRAMES_PER_BUFFER 4

float z = 10;
int32_t indexx = 1;
Filter* f;

// Callback function for the input stream
int inputCallback(const void *inputBuffer, void *outputBuffer,
                   unsigned long framesPerBuffer,
                   const PaStreamCallbackTimeInfo *timeInfo,
                   PaStreamCallbackFlags statusFlags,
                   void *userData) {
    uint32_t *out = (uint32_t*)inputBuffer;
    
    for (int i = 0; i < FRAMES_PER_BUFFER; i++)
    {
        uint32_t* a = out + i;
        z = fmax((int32_t)*a, z);
        *a *= 3;
        
        if(indexx < SAMPLE_RATE/2)
        {
            int32_t in=*((int32_t*)a), out = 0;
            f->process(in, out);
            *a = out;
        }
        //z = (*a>>31) & 0x01;
        indexx++;
        indexx=indexx%SAMPLE_RATE;

    }
    memcpy(outputBuffer, inputBuffer, framesPerBuffer * CHANNELS * sizeof(uint32_t));
    return paContinue;
}

int main() {
    PaError err;
    
    std::vector<float> vect{ 0.1, 0.1, 0.1, 0.1 };
    f = new Filter(vect);
    
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

    inputParameters.device = 0;
    inputParameters.channelCount = CHANNELS;
    inputParameters.sampleFormat = paInt32;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(1)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = nullptr;

    outputParameters.device = 0; // Use the default output device for playback
    outputParameters.channelCount = CHANNELS;
    outputParameters.sampleFormat = paInt32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
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
        printf("%f\n", z);
        z = 0;
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

#include <iostream>
#include "portaudio.h"
#include <cstring>
#include <cmath>
#include <vector>
#include "../inc/filter.h"
#include "../inc/scale.h"
#include "../inc/clip.h"
#include <stdint.h>
#include <RtMidi.h>

#define SAMPLE_RATE 192000
#define CHANNELS 1 
#define FRAMES_PER_BUFFER 4

float z = 10;
int32_t indexx = 1;
Filter* f;
Scale* s;
Clip* c;
// Callback function for the input stream
int inputCallback(const void *inputBuffer, void *outputBuffer,
                   unsigned long framesPerBuffer,
                   const PaStreamCallbackTimeInfo *timeInfo,
                   PaStreamCallbackFlags statusFlags,
                   void *userData) {
    uint32_t *out = (uint32_t*)inputBuffer;
    
    for (int i = 0; i < FRAMES_PER_BUFFER; i++)
    {
        int32_t* a = (int32_t*)(out + i);
        //z = fmax((int32_t)*a, z);
        //*a *= 5;
        s->process(*a, *a);
        

        if(indexx < SAMPLE_RATE)
        {
            //int32_t in=*((int32_t*)a), out = 0;
            //f->process(in, out);
            //*a = out;

            //c->process(*a, *a);

            z = 1;
        }
        else
            z = 0;
        //z = (*a>>31) & 0x01;
        indexx++;
        indexx=indexx%(SAMPLE_RATE*2);

    }
    memcpy(outputBuffer, inputBuffer, framesPerBuffer * CHANNELS * sizeof(uint32_t));
    return paContinue;
}


void errorCallback(RtMidiError::Type type, const std::string &errorText) {
    // Handle errors
    std::cerr << "Error: " << errorText << std::endl;
}

int main() {

    RtMidiIn midiIn;
    //midiIn.setErrorCallback(errorCallback);

    // Check available ports
    unsigned int numPorts = midiIn.getPortCount();
    std::cout << "Available MIDI ports:" << std::endl;
    for (unsigned int i = 0; i < numPorts; ++i) {
        std::cout << "  " << i << ": " << midiIn.getPortName(i) << std::endl;
    }

    if (numPorts == 0) {
        std::cerr << "No MIDI input ports found." << std::endl;
        return 1;
    }

    // Open the first available port
    midiIn.openPort(1);

    // Don't ignore sysex, timing, or active sensing messages.
    midiIn.ignoreTypes(false, false, false);

        // Read MIDI input
    PaError err;
    
    std::vector<float> vect{  
        0.006979372451474281,
        0.0007626387903709561,
        -0.08083710305842673,
        -0.18196149840834866,
        -0.06108187078417545,
        0.3184786604146606,
        0.5447090129383919,
        0.3184786604146606,
        -0.06108187078417545,
        -0.18196149840834866,
        -0.08083710305842673,
        0.0007626387903709561,
        0.006979372451474281 
        };
    f = new Filter(vect);
    s = new Scale(5);
    c = new Clip(1000000);

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
    std::vector<unsigned char> message;
    while (true) {
        Pa_Sleep(1);
        midiIn.getMessage(&message);
        // Process MIDI message
        // Example: Print MIDI message bytes
        //printf("loop\n");
        //for (unsigned int i = 0; i < message.size(); ++i) {
        //    std::cout << "Byte " << i << " = " << (int)message[i] << std::endl;
        //}

        if(message.size() >= 3)
        {
            if(message[0] == 176) // pots
            {
                if(message[1] == 23)
                {
                    s->scale = message[2] * 5 / 127;
                }

                if(message[1] == 24)
                {
                    //c->threshold = message[2] * 
                }
            }
            printf("%d\n", message[1]);
        }        
    }


    // Keep the program running until interrupted
    while (true) 
    {
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

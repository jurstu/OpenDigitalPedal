#ifndef __clip
#define __clip

#include <vector>
#include "sampleType.h"
#include <stdint.h>

class Clip
{
    public:
    Clip(float val);
    void process(SAMPLE_TYPE& in, SAMPLE_TYPE& out);

    float threshold;
};




 


#endif
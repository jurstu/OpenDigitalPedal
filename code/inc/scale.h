#ifndef __scale
#define __scale

#include <vector>
#include "sampleType.h"
#include <stdint.h>

class Scale
{
    public:
    Scale(float val);
    void process(SAMPLE_TYPE& in, SAMPLE_TYPE& out);

    float scale;
};




 


#endif
#ifndef __filter
#define __filter

#include <vector>
#include "sampleType.h"

class Filter 
{
    Filter(std::vector<float> coeff);
    void process(SAMPLE_TYPE& in, SAMPLE_TYPE& out);

    private:
        std::vector<float> filter_coefficients;
        std::vector<SAMPLE_TYPE> recent_samples;
};




 


#endif
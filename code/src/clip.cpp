#include "../inc/clip.h"


Clip::Clip(float val)
{
    threshold = val;
}

void Clip::process(SAMPLE_TYPE &in, SAMPLE_TYPE& out)
{
    if(in > threshold)
        in = threshold;
    
    if(in < -threshold)
        in = -threshold;
    
    out = in;
}

#include "../inc/scale.h"


Scale::Scale(float val)
{
    scale = val;
}



void Scale::process(SAMPLE_TYPE &in, SAMPLE_TYPE& out)
{
    out = in*scale;
}

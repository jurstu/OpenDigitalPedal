#include "../inc/filter.h"


Filter::Filter(std::vector<float> coeff)
{
    for(int i = 0; i < coeff.size(); i++)
    {
        filter_coefficients.push_back(coeff[i]);        
        recent_samples.push_back(0);
    } 
}



void Filter::process(SAMPLE_TYPE &in, SAMPLE_TYPE& out)
{
    recent_samples.insert(recent_samples.begin(), in);
    recent_samples.pop_back();
    SAMPLE_TYPE ret = 0;
    for (int i = 0; i < filter_coefficients.size(); i++)
    {
        ret += recent_samples[i] * filter_coefficients[i];   
    }
    out = ret;
}

#ifndef DROPMERGE_MERGE_FUNCTION_IMPL_HPP
#define DROPMERGE_MERGE_FUNCTION_IMPL_HPP

#include <algorithm>

namespace cie {
namespace csg {


static size_t mergeCounter;


template <size_t D>
double polynomialMergeFunction( const DoubleArray<D>& point, double offset )
{
    double result   = point[0]*point[0];
    result          = 0.25*result*result - 0.5*result;

    for (auto it=point.begin()+1; it!=point.end(); ++it)
        result  += (*it) * (*it);

    return result - offset;
}


template <size_t D>
double exponentialMergeFunction( const DoubleArray<D>& point, double offset )
{
    ++mergeCounter;

    double result = 0.0;
    std::for_each(  point.begin()+1, 
                    point.end(), 
                    [&result](auto value){result+=value*value;} );

    result =    exp( -result - (point[0]-offset)*(point[0]-offset) )
                + exp( -result - (point[0]+offset)*(point[0]+offset) );

    return 0.75 - result;
}


}
}

#endif
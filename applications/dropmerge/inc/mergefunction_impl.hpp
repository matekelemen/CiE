#ifndef DROPMERGE_MERGE_FUNCTION_IMPL_HPP
#define DROPMERGE_MERGE_FUNCTION_IMPL_HPP

namespace cie {
namespace csg {


template <uint8_t D>
double mergeFunction( const DoubleArray<D>& point, double offset )
{
    double result = point[0]*point[0];
    result *= 0.25*result*result - 0.5*result;

    for (auto it=point.begin()+1; it!=point.end(); ++it)
    {
        result  += (*it) * (*it);
    }

    return result -0.25 - offset;
}


}
}

#endif
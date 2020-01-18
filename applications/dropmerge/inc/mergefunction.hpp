#ifndef DROPMERGE_MERGE_FUNCTION_HPP
#define DROPMERGE_MERGE_FUNCTION_HPP

#include "../../libraries/CSG/trees/inc/spacetreenode.hpp"

namespace cie {
namespace csg {


template <size_t D>
double polynomialMergeFunction( const DoubleArray<D>& point, double offset=0.0 );


template <size_t D>
double exponentialMergeFunction( const DoubleArray<D>& point, double offset=0.0 );


}
}

#include "mergefunction_impl.hpp"

#endif
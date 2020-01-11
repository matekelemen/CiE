#ifndef DROPMERGE_MERGE_FUNCTION_HPP
#define DROPMERGE_MERGE_FUNCTION_HPP

#include "../../libraries/CSG/trees/inc/ntreenode.hpp"

namespace cie {
namespace csg {


template <uint8_t D>
double mergeFunction( const DoubleArray<D>& point, double offset=0.0 );


}
}

#include "mergefunction_impl.hpp"

#endif
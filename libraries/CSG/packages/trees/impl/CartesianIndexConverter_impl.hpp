#ifndef CIE_CSG_TREES_CARTESIAN_INDEX_CONVERTER_IMPL_HPP
#define CIE_CSG_TREES_CARTESIAN_INDEX_CONVERTER_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/maths/inc/base.hpp"
#include "cieutils/packages/maths/inc/power.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"

// --- STL Includes ---
#include <iterator>


namespace cie::csg {


template <Size Dimension>
CartesianIndexConverter<Dimension>::CartesianIndexConverter( Size numberOfPointsPerDimension )
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->setNumberOfPointsPerDimension( numberOfPointsPerDimension );

    CIE_END_EXCEPTION_TRACING
}


template <Size Dimension>
inline const typename CartesianIndexConverter<Dimension>::output_index_type&
CartesianIndexConverter<Dimension>::convert( Size index ) const
{
    CIE_OUT_OF_RANGE_CHECK( index < _gridIndices.size() )

    return _gridIndices[index];
}


template <Size Dimension>
inline Size
CartesianIndexConverter<Dimension>::numberOfPointsPerDimension() const
{
    return this->_numberOfPointsPerDimension;
}


template <Size Dimension>
inline Size
CartesianIndexConverter<Dimension>::numberOfPoints() const
{
    return this->_numberOfPoints;
}


template <Size Dimension>
void
CartesianIndexConverter<Dimension>::setNumberOfPointsPerDimension( Size numberOfPointsPerDimension )
{
    CIE_BEGIN_EXCEPTION_TRACING

    _numberOfPointsPerDimension = numberOfPointsPerDimension;
    _numberOfPoints             = intPow( numberOfPointsPerDimension, Dimension );

    cie::utils::resize( _gridIndices, _numberOfPoints );
    for ( Size i=0; i<_numberOfPoints; ++i )
        utils::baseN( i, numberOfPointsPerDimension, _gridIndices[i] );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::csg

#endif
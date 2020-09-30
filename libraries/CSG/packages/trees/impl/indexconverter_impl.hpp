#ifndef CSG_NTREE_INDEX_CONVERTER_IMPL_HPP
#define CSG_NTREE_INDEX_CONVERTER_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/maths/inc/base.hpp"
#include "cieutils/packages/maths/inc/power.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"

// --- STL Includes ---
#include <iterator>


namespace cie::csg {


template <Size Dimension>
GridIndexConverter<Dimension>::GridIndexConverter( Size numberOfPointsPerDimension )
{
    this->setNumberOfPointsPerDimension( numberOfPointsPerDimension );
}


template <Size Dimension>
inline const UIntArray<Dimension>&
GridIndexConverter<Dimension>::convert( Size index ) const
{
    CIE_OUT_OF_RANGE_ASSERT(
        index < _gridIndices.size(),
        "GridIndexConverter::convert"
    )

    return _gridIndices[index];
}


template <Size Dimension>
inline Size
GridIndexConverter<Dimension>::numberOfPointsPerDimension() const
{
    return this->_numberOfPointsPerDimension;
}


template <Size Dimension>
inline Size
GridIndexConverter<Dimension>::numberOfPoints() const
{
    return this->_numberOfPoints;
}


template <Size Dimension>
inline GridIndexConverter<Dimension>&
GridIndexConverter<Dimension>::setNumberOfPointsPerDimension( Size numberOfPointsPerDimension )
{
    _numberOfPointsPerDimension = numberOfPointsPerDimension;
    _numberOfPoints             = intPow( numberOfPointsPerDimension, Dimension );

    cie::utils::resize( _gridIndices, _numberOfPoints );
    for ( Size i=0; i<_numberOfPoints; ++i )
        utils::baseN( i, Dimension, _gridIndices[i] );

    return *this;
}


} // namespace cie::csg

#endif
#ifndef CIE_CSG_TREES_CARTESIAN_INDEX_CONVERTER_HPP
#define CIE_CSG_TREES_CARTESIAN_INDEX_CONVERTER_HPP

// --- Utility Includes ---
#include "cieutils/packages/types/inc/types.hpp"

// --- STL Includes ---
#include <vector>
#include <memory>


namespace cie::csg {


template <Size Dimension>
class CartesianIndexConverter
{
public:
    using output_index_type      = std::array<Size,Dimension>;
    using output_index_container = std::vector<output_index_type>;

public:
    CartesianIndexConverter( Size numberOfPointsPerDimension );

    /**
     * Convert input to base numberOfPointsPerDimension
    */
    const output_index_type& convert( Size index ) const;

    //bool increment( UIntArray<Dimension>& r_index ) const;
    //bool decrement( UIntArray<Dimension>& r_index ) const;

    Size numberOfPointsPerDimension() const;
    Size numberOfPoints() const;

    void setNumberOfPointsPerDimension( Size numberOfPointsPerDimension );

private:
    Size                   _numberOfPointsPerDimension;
    Size                   _numberOfPoints;
    output_index_container _gridIndices;
};


template<Size Dimension>
using CartesianIndexConverterPtr = std::shared_ptr<CartesianIndexConverter<Dimension>>;


} // namespace cie::csg

#include "CSG/packages/trees/impl/CartesianIndexConverter_impl.hpp"

#endif
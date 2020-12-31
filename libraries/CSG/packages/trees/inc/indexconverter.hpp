#ifndef CSG_NTREE_INDEX_CONVERTER_HPP
#define CSG_NTREE_INDEX_CONVERTER_HPP

// --- Linalg Includes ---
#include "linalg/packages/types/inc/arraytypes.hpp"

// --- STL Includes ---
#include <vector>
#include <memory>


namespace cie::csg {


template <Size Dimension>
class GridIndexConverter
{
public:
    GridIndexConverter( Size numberOfPointsPerDimension );

    /**
     * Convert input to base numberOfPointsPerDimension
    */
    const UIntArray<Dimension>& convert( Size index ) const;

    //bool increment( UIntArray<Dimension>& r_index ) const;
    //bool decrement( UIntArray<Dimension>& r_index ) const;

    Size numberOfPointsPerDimension() const;
    Size numberOfPoints() const;

    GridIndexConverter<Dimension>& setNumberOfPointsPerDimension( Size numberOfPointsPerDimension );

private:
    Size                                _numberOfPointsPerDimension;

    Size                                _numberOfPoints;
    std::vector<UIntArray<Dimension>>   _gridIndices;
};


template<Size Dimension>
using GridIndexConverterPtr = std::shared_ptr<GridIndexConverter<Dimension>>;


} // namespace cie::csg

#include "CSG/packages/trees/impl/indexconverter_impl.hpp"

#endif
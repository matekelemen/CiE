#ifndef CIE_CSG_PARTITIONING_ABS_BOUNDABLE_OBJECT_HPP
#define CIE_CSG_PARTITIONING_ABS_BOUNDABLE_OBJECT_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"

// --- Internal Includes ---
#include "CSG/packages/partitioning/inc/AABBox.hpp"

// --- STL Includes ---
#include <concepts>


namespace cie::csg {


template < Size Dimension,
           concepts::NumericType CoordinateType >
class AbsBoundableObject
{
public:
    using bounding_box = AABBox<Dimension,CoordinateType>;

public:
    AbsBoundableObject();
    AbsBoundableObject( const AbsBoundableObject<Dimension,CoordinateType>& r_rhs ) = default;
    AbsBoundableObject<Dimension,CoordinateType>& operator=( const AbsBoundableObject<Dimension,CoordinateType>& r_rhs ) = default;

    const bounding_box& boundingBox();
    const bounding_box& boundingBox() const;

protected:
    void boundingBoxShouldRecompute();

    void computeBoundingBox();
    virtual void computeBoundingBox_impl( bounding_box& r_box ) = 0;

private:
    bool         _recomputeBoundingBox;
    bounding_box _boundingBox;
};


} // namespace cie::csg



/* --- BoxBoundable Concept --- */

namespace cie::concepts {
template <class T>
concept BoxBoundable
= requires ( T instance, const T constInstance )
{
    { instance.boundingBox() };
    { constInstance.boundingBox() };
};
} // namespace cie::concepts



#include "CSG/packages/partitioning/impl/AbsBoundableObject_impl.hpp"

#endif
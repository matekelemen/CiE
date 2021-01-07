#ifndef CIE_CSG_TREES_ABS_CELL_HPP
#define CIE_CSG_TREES_ABS_CELL_HPP

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/Primitive.hpp"
#include "CSG/packages/primitives/inc/concepts.hpp"

// --- Utility Includes ---
#include "cieutils/packages/trees/inc/abstree.hpp"
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"
#include "cieutils/packages/concepts/inc/partial_specialization.hpp"
#include "cieutils/packages/types/inc/types.hpp"

// --- STL Includes ---
#include <memory>
#include <vector>

namespace cie::csg {

/**
 * Node interface of a tree of primitives, splittable around a single point.
 * The 'split' interface supplies a set of tuples necessary for constructing
 * a set of primitives (results of the split).
*/
template <concepts::Primitive PrimitiveType>
class AbsCell : public PrimitiveType
{
public:
    using primitive_type                        = PrimitiveType;
    using cell_base_type                        = AbsCell<PrimitiveType>;
    
    using primitive_constructor_container       = std::vector<typename primitive_type::primitive_constructor_arguments>;
    using primitive_constructor_container_ptr   = std::shared_ptr<primitive_constructor_container>;

public:

    /**
     * Construct a cell by forwarding incoming arguments to the
     * primitive constructor.
    */
    template <class ...Args>
    AbsCell( Args&&... args );

    /**
     * Split overload that doesn't attempt to convert the coordinate container
     * to point_type (as it is already point_type).
    */
    primitive_constructor_container_ptr split( const typename AbsCell<PrimitiveType>::point_type& point );

protected:

    /**
     * Split the primitive around a point and return a container of tuples
     * that hold the arguments for constructing the subcells.
    */
    virtual primitive_constructor_container_ptr split_internal( const typename AbsCell<PrimitiveType>::point_type& point ) = 0;
};


CIE_DEFINE_INVALID_CLASS_TEMPLATE_TO_SPECIALIZE( Cell )


} // namespace cie::csg

#include "CSG/packages/trees/impl/AbsCell_impl.hpp"

#endif
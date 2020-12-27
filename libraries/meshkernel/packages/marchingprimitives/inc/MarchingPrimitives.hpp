#ifndef CIE_MESHKERNEL_MARCHING_PRIMITIVES_HPP
#define CIE_MESHKERNEL_MARCHING_PRIMITIVES_HPP

// --- CSG Includes ---
#include "CSG/packages/primitives/inc/csgobject.hpp"
#include "CSG/packages/primitives/inc/CSGTraits.hpp"
#include "CSG/packages/primitives/inc/concepts.hpp"

// --- Internal Includes ---
#include "meshkernel/packages/marchingprimitives/inc/connectivitytables.hpp"

// --- STL Includes ---
#include <vector>
#include <array>
#include <memory>


namespace cie::mesh {


template < concepts::CSGObject TargetType,
           concepts::Primitive PrimitiveType >
class MarchingPrimitives
{
public:
    static const Size dimension = csg::Traits<TargetType>::dimension;
    using coordinate_type       = typename csg::Traits<TargetType>::coordinate_type;
    using point_type            = typename csg::Traits<TargetType>::

    using target_type           = TargetType;
    using primitive_type        = PrimitiveType;

    using target_ptr            = std::shared_ptr<target_type>;
    using primitive_ptr         = std::shared_ptr<primitive_type>;

    using point_container       = std::vector<point_type>;
    using primitive_container   = std::vector<primitive_type>;

    using connectivity_table    = std::vector<std::vector<Size>>;

    using output_arguments      = std::array<point_type,dimension>;
    using output_functor        = std::function<void(const output_arguments&)>;

public:
    

public:
    MarchingPrimitives( target_ptr p_target,
                        const connectivity_table& r_connectivityTable,
                        output_functor outputFunctor );

protected:
    virtual bool nextPrimitiveSet( primitive_container& r_primitives ) = 0;

protected:
    MarchingPrimitives() = delete;
    MarchingPrimitives( const MarchingPrimitives<TargetType,PrimitiveType>& r_rhs ) = delete;
    MarchingPrimitives<TargetType,PrimitiveType>& operator=( const MarchingPrimitives<TargetType,PrimitiveType>& r_rhs ) = delete;

protected:
    target_ptr                _p_target;
    const connectivity_table& _r_connectivityTable;
    output_functor            _outputFunctor;
};


} // namespace cie::mesh

#endif
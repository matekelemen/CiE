#ifndef CIE_MESHKERNEL_MARCHING_PRIMITIVES_HPP
#define CIE_MESHKERNEL_MARCHING_PRIMITIVES_HPP

// --- Utility Includes ---
#include "cieutils/packages/stl_extension/inc/MarchingContainer.hpp"

// --- CSG Includes ---
#include "CSG/packages/primitives/inc/csgobject.hpp"
#include "CSG/packages/primitives/inc/CSGTraits.hpp"
#include "CSG/packages/primitives/inc/concepts.hpp"

// --- Internal Includes ---
#include "meshkernel/packages/marchingprimitives/inc/connectivitytables.hpp"
#include "meshkernel/packages/traits/inc/MeshTraits.hpp"

// --- STL Includes ---
#include <vector>
#include <array>
#include <memory>
#include <utility>


namespace cie::mesh {


template <concepts::CSGObject TargetType>
class MarchingPrimitives : public Traits<TargetType>
{
public:
    using target_type           = TargetType;
    using target_ptr            = std::shared_ptr<target_type>;

    using point_container       = std::vector<typename MarchingPrimitives<TargetType>::point_type>;

    using edge_table            = std::vector<std::pair<Size,Size>>;
    using connectivity_table    = std::vector<std::vector<std::vector<Size>>>;

    using output_arguments      = std::array<typename MarchingPrimitives<TargetType>::point_type,MarchingPrimitives<TargetType>::dimension>;
    using output_functor        = std::function<void(const output_arguments&)>;

public:
    MarchingPrimitives( target_ptr p_target,
                        const edge_table& r_edgeTable,
                        const connectivity_table& r_connectivityTable,
                        output_functor outputFunctor );

    void execute();

private:
    /// Make sure that everything is set up to perform the march
    void checkIfInitialized() const;

protected:
    MarchingPrimitives() = delete;
    MarchingPrimitives( const MarchingPrimitives<TargetType>& r_rhs ) = delete;
    MarchingPrimitives<TargetType>& operator=( const MarchingPrimitives<TargetType>& r_rhs ) = delete;

    /// Primitive generator
    virtual bool getNextPrimitive( point_container& r_vertices ) = 0;

protected:
    /// Pointer to the target geometry
    target_ptr                _p_target;

    /// Map edge index to vertex pairs
    const edge_table          _edgeTable;

    /// Map active point configurations to surface primitives (3D: active point config -> triangles)
    const connectivity_table  _connectivityTable;

    /// Function that gets called for every surface primitive
    output_functor            _outputFunctor;
};


} // namespace cie::mesh

#include "meshkernel/packages/marchingprimitives/impl/MarchingPrimitives_impl.hpp"

#endif
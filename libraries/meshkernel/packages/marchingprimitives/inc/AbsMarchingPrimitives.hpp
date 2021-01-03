#ifndef CIE_MESHKERNEL_ABS_MARCHING_PRIMITIVES_HPP
#define CIE_MESHKERNEL_ABS_MARCHING_PRIMITIVES_HPP

// --- Utility Includes ---
#include "cieutils/packages/stl_extension/inc/MarchingContainer.hpp"

// --- CSG Includes ---
#include "CSG/packages/primitives/inc/Primitive.hpp"
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
class AbsMarchingPrimitives : public Traits<TargetType>
{
public:
    using target_type           = TargetType;
    using target_ptr            = std::shared_ptr<target_type>;

    using edge_type             = std::pair<Size,Size>;
    using edge_table            = std::vector<edge_type>;
    using connectivity_table    = std::vector<std::vector<std::vector<Size>>>;

    /// Edge indices for every output vertex
    using output_arguments      = std::array<edge_type,AbsMarchingPrimitives<TargetType>::dimension>;

    /// Primitive index and output_arguments
    using output_functor        = std::function<void(Size,const output_arguments&)>;

public:
    AbsMarchingPrimitives( target_ptr p_target,
                           const edge_table& r_edgeTable,
                           const connectivity_table& r_connectivityTable,
                           output_functor outputFunctor );

    void execute();

    void setOutputFunctor( output_functor outputFunctor );

private:
    /// Make sure that everything is set up to perform the march
    void checkIfInitialized() const;

protected:
    AbsMarchingPrimitives() = delete;
    AbsMarchingPrimitives( const AbsMarchingPrimitives<TargetType>& r_rhs ) = delete;
    AbsMarchingPrimitives<TargetType>& operator=( const AbsMarchingPrimitives<TargetType>& r_rhs ) = delete;

    /// Vertex generator
    virtual typename AbsMarchingPrimitives<TargetType>::point_type getVertex( Size primitiveIndex,
                                                                              Size vertexIndex ) = 0;

    /// Return the number of vertices that form a primitive
    virtual Size primitiveVertexCount() const = 0;

    /// Return the number of remaining primitives to be scanned
    virtual Size numberOfRemainingPrimitives() const = 0;

protected:
    /// Pointer to the target geometry
    target_ptr                _p_target;

    /// Map edge index to vertex pairs
    edge_table                _edgeTable;

    /// Map active point configurations to surface primitives (3D: active point config -> triangles)
    connectivity_table        _connectivityTable;

    /// Function that gets called for every surface primitive
    output_functor            _outputFunctor;
};



template <concepts::CSGObject TargetType>
using MarchingPrimitivesPtr = std::shared_ptr<AbsMarchingPrimitives<TargetType>>;


} // namespace cie::mesh

#include "meshkernel/packages/marchingprimitives/impl/AbsMarchingPrimitives_impl.hpp"

#endif
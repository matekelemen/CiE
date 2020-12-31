#ifndef CIE_MESH_KERNEL_CARTESIAN_MESH_HPP
#define CIE_MESH_KERNEL_CARTESIAN_MESH_HPP

// --- CSG Includes ---
#include "CSG/packages/primitives/inc/Cube.hpp"
#include "CSG/packages/primitives/inc/Box.hpp"

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- Internal Includes ---
#include "meshkernel/packages/traits/inc/MeshTraits.hpp"

// --- STL Includes ---
#include <vector>
#include <functional>


namespace cie::mesh {


/// Generate a cartesian mesh of cube elements into an existing container
template < concepts::Cube PrimitiveType,
           concepts::STLContainer PrimitiveContainer >
void makeCartesianMesh( const typename Traits<PrimitiveType>::resolution_specifier& r_numberOfPrimitives,
                        typename Traits<PrimitiveType>::coordinate_type edgeLength,
                        const typename PrimitiveType::point_type& r_origin,
                        PrimitiveContainer& r_primitives );


/// Generate a cartesian mesh of cube elements
template < concepts::Cube PrimitiveType,
           concepts::STLContainer PrimitiveContainer = std::vector<std::shared_ptr<PrimitiveType>> >
PrimitiveContainer makeCartesianMesh( const typename Traits<PrimitiveType>::resolution_specifier& r_numberOfPrimitives,
                                      typename Traits<PrimitiveType>::coordinate_type edgeLength,
                                      const typename PrimitiveType::point_type& r_origin = csg::detail::makeOrigin<Traits<PrimitiveType>::dimension,typename Traits<PrimitiveType>::coordinate_type>() );


/// Generate a cartesian mesh of box elements into an existing container
template < concepts::Box PrimitiveType,
           concepts::STLContainer PrimitiveContainer >
void makeCartesianMesh( const typename Traits<PrimitiveType>::domain_specifier& r_domain,
                        const typename Traits<PrimitiveType>::resolution_specifier& r_resolution,
                        PrimitiveContainer& r_primitives );


/// Generate a cartesian mesh of box elements
template < concepts::Box PrimitiveType,
           concepts::STLContainer PrimitiveContainer = std::vector<std::shared_ptr<PrimitiveType>> >
PrimitiveContainer makeCartesianMesh( const typename Traits<PrimitiveType>::domain_specifier& r_domain,
                                      const typename Traits<PrimitiveType>::resolution_specifier& r_resolution );





// Helper functions for emplacing back
// primitives or primitive pointers into a container
namespace detail {

template <concepts::NonPointerContainer ContainerType, class ...Args>
void constructPrimitiveIntoContainer( ContainerType& r_container,
                                      Args&&... args );

template <concepts::PointerContainer ContainerType, class ...Args>
void constructPrimitiveIntoContainer( ContainerType& r_container,
                                      Args&&... args );

} // namespace detail


} // namespace cie::mesh

#include "meshkernel/packages/structured/impl/cartesianmesh_impl.hpp"

#endif
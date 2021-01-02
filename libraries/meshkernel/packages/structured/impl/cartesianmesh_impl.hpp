#ifndef CIE_MESH_KERNEL_CARTESIAN_MESH_IMPL_HPP
#define CIE_MESH_KERNEL_CARTESIAN_MESH_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/stl_extension/inc/state_iterator.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- STL Includes ---
#include <algorithm>
#include <numeric>


namespace cie::mesh {


template < concepts::NumericContainer PointType,
           concepts::STLContainer PointContainer >
requires std::derived_from<PointType,typename Traits<PointType>::point_type>
void makeCartesianMesh( const typename Traits<PointType>::domain_specifier& r_domain,
                        const typename Traits<PointType>::resolution_specifier& r_resolution,
                        PointContainer& r_points )
{
    CIE_BEGIN_EXCEPTION_TRACING

    const Size dimension = Traits<PointType>::dimension;

    // Allocate memory for the points
    Size numberOfPoints = std::accumulate(
        r_resolution.begin(),
        r_resolution.end(),
        1.0,
        []( Size lhs, Size rhs ) -> Size { return lhs * rhs; }
    );

    utils::reserve( r_points, r_points.size() + numberOfPoints );

    // Get mesh origin
    PointType origin;
    utils::resize( origin, dimension );
    for ( Size dim=0; dim<dimension; ++dim )
        origin[dim] = r_domain[dim].first;

    // Compute edge lengths
    PointType edgeLengths;
    utils::resize( edgeLengths, dimension );
    for ( Size dim=0; dim<dimension; ++dim )
        edgeLengths[dim] = (r_domain[dim].second - r_domain[dim].first) / (r_resolution[dim] - 1);

    // Create iterator through the cartesian products
    std::vector<std::vector<Size>> cartesianStates( dimension );

    for ( Size dim=0; dim<dimension; ++dim )
    {
        auto& r_range = cartesianStates[dim];

        r_range.resize( r_resolution[dim] );

        std::iota( r_range.begin(),
                   r_range.end(),
                   0 );
    }

    auto cartesianProductIterator = utils::makeStateIterator( cartesianStates );

    // Construct mesh
    for ( Size pointIndex=0; pointIndex<numberOfPoints; ++pointIndex, ++cartesianProductIterator )
    {
        const auto& r_spatialIndex = *cartesianProductIterator;
        PointType point = origin;

        for ( Size dim=0; dim<dimension; ++dim )
            point[dim] += (*r_spatialIndex[dim]) * edgeLengths[dim];

        detail::constructPrimitiveIntoContainer( r_points,
                                                 point );
    }

    CIE_END_EXCEPTION_TRACING
}


template < concepts::NumericContainer PointType,
           concepts::STLContainer PointContainer >
requires std::derived_from<PointType,typename Traits<PointType>::point_type>
PointContainer makeCartesianMesh( const typename Traits<PointType>::domain_specifier& r_domain,
                                  const typename Traits<PointType>::resolution_specifier& r_resolution )
{
    CIE_BEGIN_EXCEPTION_TRACING

    PointContainer points;
    
    makeCartesianMesh<PointType>( r_domain,
                                  r_resolution,
                                  points );

    return points;

    CIE_END_EXCEPTION_TRACING
}


template < concepts::Cube PrimitiveType,
           concepts::STLContainer PrimitiveContainer >
void makeCartesianMesh( const typename Traits<PrimitiveType>::resolution_specifier& r_numberOfPrimitives,
                        typename Traits<PrimitiveType>::coordinate_type edgeLength,
                        const typename PrimitiveType::point_type& r_origin,
                        PrimitiveContainer& r_primitives )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Allocate memory for the primitives
    Size numberOfPrimitives = std::accumulate(
        r_numberOfPrimitives.begin(),
        r_numberOfPrimitives.end(),
        1.0,
        []( Size lhs, Size rhs ) -> Size { return lhs * rhs; }
    );

    if ( numberOfPrimitives == 0 )
        return;

    utils::reserve( r_primitives, r_primitives.size() + numberOfPrimitives );

    // Create iterator through the cartesian products
    std::vector<std::vector<Size>> cartesianStates( Traits<PrimitiveType>::dimension );

    for ( Size dim=0; dim<Traits<PrimitiveType>::dimension; ++dim )
    {
        auto& r_range = cartesianStates[dim];

        r_range.resize( r_numberOfPrimitives[dim] );

        std::iota( r_range.begin(),
                   r_range.end(),
                   0 );
    }

    auto cartesianProductIterator = utils::makeStateIterator( cartesianStates );

    // Construct primitives
    for ( Size primitiveIndex=0; primitiveIndex<numberOfPrimitives; ++primitiveIndex, ++cartesianProductIterator )
    {
        const auto& r_spatialIndex = *cartesianProductIterator;
        auto base = r_origin;

        for ( Size dim=0; dim<Traits<PrimitiveType>::dimension; ++dim )
            base[dim] += (*r_spatialIndex[dim]) * edgeLength;

        detail::constructPrimitiveIntoContainer( r_primitives,
                                                 base, edgeLength );
    }

    CIE_END_EXCEPTION_TRACING
}


template < concepts::Cube PrimitiveType,
           concepts::STLContainer PrimitiveContainer >
PrimitiveContainer makeCartesianMesh( const typename Traits<PrimitiveType>::resolution_specifier& r_numberOfPrimitives,
                                      typename Traits<PrimitiveType>::coordinate_type edgeLength,
                                      const typename PrimitiveType::point_type& r_origin )
{
    CIE_BEGIN_EXCEPTION_TRACING

    PrimitiveContainer primitives;
    
    makeCartesianMesh<PrimitiveType>( r_numberOfPrimitives,
                                      edgeLength,
                                      r_origin,
                                      primitives );

    return primitives;

    CIE_END_EXCEPTION_TRACING
}


template < concepts::Box PrimitiveType,
           concepts::STLContainer PrimitiveContainer >
void makeCartesianMesh( const typename Traits<PrimitiveType>::domain_specifier& r_domain,
                        const typename Traits<PrimitiveType>::resolution_specifier& r_resolution,
                        PrimitiveContainer& r_primitives )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Init
    const Size dimension = Traits<PrimitiveType>::dimension;
    using PointType      = typename Traits<PrimitiveType>::point_type;

    // Allocate memory for the primitives
    Size numberOfPrimitives = std::accumulate(
        r_resolution.begin(),
        r_resolution.end(),
        1.0,
        []( Size lhs, Size rhs ) -> Size { return lhs * rhs; }
    );

    if ( numberOfPrimitives == 0 )
        return;

    utils::reserve( r_primitives, r_primitives.size() + numberOfPrimitives );

    // Compute edge lengths
    PointType edgeLengths;
    utils::resize( edgeLengths, dimension );
    for ( Size dim=0; dim<dimension; ++dim )
        edgeLengths[dim] = (r_domain[dim].second - r_domain[dim].first) / r_resolution[dim];

    // Get mesh origin
    PointType origin;
    utils::resize( origin, dimension );
    for ( Size dim=0; dim<dimension; ++dim )
        origin[dim] = r_domain[dim].first;

    // Create iterator through the cartesian products
    std::vector<std::vector<Size>> cartesianStates( dimension );

    for ( Size dim=0; dim<dimension; ++dim )
    {
        auto& r_range = cartesianStates[dim];

        r_range.resize( r_resolution[dim] );

        std::iota( r_range.begin(),
                   r_range.end(),
                   0 );
    }

    auto cartesianProductIterator = utils::makeStateIterator( cartesianStates );

    // Construct primitives
    for ( Size primitiveIndex=0; primitiveIndex<numberOfPrimitives; ++primitiveIndex, ++cartesianProductIterator )
    {
        const auto& r_spatialIndex = *cartesianProductIterator;
        auto base = origin;

        for ( Size dim=0; dim<Traits<PrimitiveType>::dimension; ++dim )
            base[dim] += (*r_spatialIndex[dim]) * edgeLengths[dim];

        detail::constructPrimitiveIntoContainer( r_primitives,
                                                 base, edgeLengths );
    }

    CIE_END_EXCEPTION_TRACING
}


template < concepts::Box PrimitiveType,
           concepts::STLContainer PrimitiveContainer >
PrimitiveContainer makeCartesianMesh( const typename Traits<PrimitiveType>::domain_specifier& r_domain,
                                      const typename Traits<PrimitiveType>::resolution_specifier& r_resolution )
{
    CIE_BEGIN_EXCEPTION_TRACING

    PrimitiveContainer primitives;

    makeCartesianMesh<PrimitiveType>( r_domain,
                                      r_resolution,
                                      primitives );

    return primitives;

    CIE_END_EXCEPTION_TRACING
}





namespace detail {


template <concepts::NonPointerContainer ContainerType, class ...Args>
void constructPrimitiveIntoContainer( ContainerType& r_container,
                                      Args&&... args )
{
    CIE_BEGIN_EXCEPTION_TRACING

    r_container.emplace_back(
        std::forward<Args>(args)...
    );

    CIE_END_EXCEPTION_TRACING
}


template <concepts::PointerContainer ContainerType, class ...Args>
void constructPrimitiveIntoContainer( ContainerType& r_container,
                                      Args&&... args )
{
    CIE_BEGIN_EXCEPTION_TRACING

    r_container.emplace_back( new typename ContainerType::value_type::element_type(
        std::forward<Args>(args)...
    ));

    CIE_END_EXCEPTION_TRACING
}


} // namespace detail


} // namespace cie::mesh


#endif
#ifndef CIE_CSG_PARTITIONING_BOX_FILE_IMPL_HPP
#define CIE_CSG_PARTITIONING_BOX_FILE_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"

// --- STL Includes ---
#include <memory>


namespace cie::csg {


template <concepts::Cube PrimitiveType>
BoxFile& operator<<( BoxFile& r_stream, const PrimitiveType& r_cube )
{
    CIE_BEGIN_EXCEPTION_TRACING

    CIE_CHECK(
        r_stream._dimension == PrimitiveType::dimension,
        "Dimension mismatch: " + std::to_string(r_stream._dimension) + " != " + std::to_string(PrimitiveType::dimension)
    )

    CIE_CHECK(
        r_stream._coordinateByteSize == sizeof(typename PrimitiveType::coordinate_type),
        "Coordinate type byte size mismatch: " + std::to_string( r_stream._coordinateByteSize ) + " != " + std::to_string( sizeof(typename PrimitiveType::coordinate_type) )
    )

    for ( Size dim=0; dim<r_stream._dimension; ++dim )
        r_stream._p_file->write( reinterpret_cast<const char*>(&r_cube.base()[dim]), r_stream._coordinateByteSize );

    for ( Size dim=0; dim<r_stream._dimension; ++dim )
    {
        typename PrimitiveType::coordinate_type tmp = r_cube.base()[dim] + r_cube.length();
        r_stream._p_file->write( reinterpret_cast<const char*>( &tmp ), r_stream._coordinateByteSize );
    }

    return r_stream;

    CIE_END_EXCEPTION_TRACING
}


template <concepts::Box PrimitiveType>
BoxFile& operator<<( BoxFile& r_stream, const PrimitiveType& r_box )
{
    CIE_BEGIN_EXCEPTION_TRACING

    CIE_CHECK(
        r_stream._dimension == PrimitiveType::dimension,
        "Dimension mismatch: " + std::to_string(r_stream._dimension) + " != " + std::to_string(PrimitiveType::dimension)
    )

    CIE_CHECK(
        r_stream._coordinateByteSize == sizeof(typename PrimitiveType::coordinate_type),
        "Coordinate type byte size mismatch: " + std::to_string( r_stream._coordinateByteSize ) + " != " + std::to_string( sizeof(typename PrimitiveType::coordinate_type) )
    )

    for ( Size dim=0; dim<r_stream._dimension; ++dim )
        r_stream._p_file->write( reinterpret_cast<char*>(&r_box.base()[dim]), r_stream._coordinateByteSize );

    for ( Size dim=0; dim<r_stream._dimension; ++dim )
    {
        typename PrimitiveType::coordinate_type tmp = r_box.base()[dim] + r_box.lengths()[dim];
        r_stream._p_file->write( reinterpret_cast<char*>( &tmp ), r_stream._coordinateByteSize );
    }

    return r_stream;

    CIE_END_EXCEPTION_TRACING
}



template <concepts::PrimitiveContainer ContainerType>
BoxFile& operator<<( BoxFile& r_stream, const ContainerType& r_primitives )
{
    CIE_BEGIN_EXCEPTION_TRACING

    for ( const auto& r_primitive : r_primitives )
        r_stream << r_primitive;

    return r_stream;

    CIE_END_EXCEPTION_TRACING
}



template <concepts::PrimitivePtrContainer ContainerType>
BoxFile& operator<<( BoxFile& r_stream, const ContainerType& r_primitives )
{
    CIE_BEGIN_EXCEPTION_TRACING

    for ( const auto& rp_primitive : r_primitives )
        r_stream << *rp_primitive;

    return r_stream;

    CIE_END_EXCEPTION_TRACING
}


template <concepts::Box PrimitiveType>
BoxFile& operator>>( BoxFile& r_stream, PrimitiveType& r_box )
{
    CIE_BEGIN_EXCEPTION_TRACING

    CIE_CHECK(
        r_box.dimension == r_stream._dimension,
        "Dimension mismatch " + std::to_string(r_box.dimension) + " (" + std::to_string(r_stream._dimension) + ")"
    )

    CIE_CHECK(
        sizeof( typename PrimitiveType::coordinate_type ) == r_stream._coordinateByteSize,
        "Coordinate byte size mismatch " + std::to_string(sizeof( typename PrimitiveType::coordinate_type )) + " (" + std::to_string(r_stream._coordinateByteSize) + ")"
    )

    for ( Size dim=0; dim<r_stream._dimension; ++dim )
        r_stream._p_file->read(
            reinterpret_cast<char*>( &r_box.base()[dim] ),
            r_stream._coordinateByteSize
        );

    for ( Size dim=0; dim<r_stream._dimension; ++dim )
    {
        r_stream._p_file->read(
            reinterpret_cast<char*>( &r_box.lengths()[dim] ),
            r_stream._coordinateByteSize
        );

        r_box.lengths()[dim] -= r_box.base()[dim];
    }

    return r_stream;

    CIE_END_EXCEPTION_TRACING
}


template <concepts::PrimitiveContainer ContainerType>
BoxFile& operator>>( BoxFile& r_stream, ContainerType& r_container )
{
    CIE_BEGIN_EXCEPTION_TRACING

    while ( r_stream._p_file->peek() != EOF )
    {
        r_container.emplace_back();
        r_stream >> r_container.back();
    }

    return r_stream;

    CIE_END_EXCEPTION_TRACING
}


template <concepts::PrimitivePtrContainer ContainerType>
BoxFile& operator>>( BoxFile& r_stream, ContainerType& r_container )
{
    CIE_BEGIN_EXCEPTION_TRACING

    while ( r_stream._p_file->peek() != EOF )
    {
        r_container.emplace_back(
            new typename std::pointer_traits<typename ContainerType::value_type>::element_type
        );
        r_stream >> *r_container.back();
    }

    return r_stream;

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::csg


#endif
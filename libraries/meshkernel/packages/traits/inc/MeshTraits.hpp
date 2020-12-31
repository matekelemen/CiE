#ifndef CIE_MESH_MESH_TRAITS_HPP
#define CIE_MESH_MESH_TRAITS_HPP

// --- CSG Includes ---
#include "CSG/packages/primitives/inc/CSGTraits.hpp"

// --- STL Includes ---
#include <array>
#include <utility>


// ---------------------------------------------------------
// MeshTraits
// ---------------------------------------------------------
namespace cie::mesh {


template <Size Dimension, concepts::NumericType CoordinateType>
struct MeshTraits : public csg::CSGTraits<Dimension,CoordinateType>
{
    using domain_specifier     = std::array<std::pair<CoordinateType,CoordinateType>,Dimension>;
    using resolution_specifier = std::array<Size,Dimension>;
};


} // namespace cie::mesh


// ---------------------------------------------------------
// CONCEPTS
// ---------------------------------------------------------
namespace cie::concepts {


namespace detail {

template <class T, class _ = void>
struct isMeshObject : std::false_type {};

template <class ...Args>
struct isMeshObjectHelper {};

template <class T>
struct isMeshObject<
    T,
    std::conditional_t<
        false,
        isMeshObjectHelper<
            typename T::coordinate_type,
            typename T::point_type,
            typename T::domain_specifier,
            typename T::resolution_specifier
        >,
        void
    >
> : public std::true_type {};

} // namespace detail


template <class T>
concept MeshObject
= detail::isMeshObject<T>::value;


} // namespace cie::concepts


// ---------------------------------------------------------
// TRAIT GENERATORS
// ---------------------------------------------------------
namespace cie::mesh {


namespace detail {

template <class T>
struct Traits
{
    using mesh_traits = void;
};

template <concepts::CSGObject ObjectType>
struct Traits<ObjectType>
{
    using mesh_traits = MeshTraits<ObjectType::dimension,typename ObjectType::coordinate_type>;
};

template <concepts::STLArray PointType>
struct Traits<PointType>
{
    using mesh_traits = MeshTraits<std::tuple_size<PointType>::value,typename PointType::value_type>;
};

template <class T>
concept HasMeshTraits
= !std::is_same_v<
    typename Traits<T>::mesh_traits,
    void
>;

} // namespace detail


template <detail::HasMeshTraits ObjectType>
struct Traits : public detail::Traits<ObjectType>::mesh_traits
{};


} // namespace cie::mesh


#endif
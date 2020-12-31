#ifndef CIE_CSG_CSG_TRAITS_HPP
#define CIE_CSG_CSG_TRAITS_HPP

// --- Utility Includes ---
#include "cieutils/packages/types/inc/types.hpp"
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- STL Includes ---
#include <array>


// ---------------------------------------------------------
// CSGTraits
// ---------------------------------------------------------
namespace cie::csg {

/// Basic templated traits that every object has in CSG
template <Size Dimension, concepts::NumericType CoordinateType>
class CSGTraits
{
public:
    static const Size                               dimension = Dimension;
    typedef CoordinateType                          coordinate_type;
    typedef std::array<CoordinateType,Dimension>    point_type;
};


template <Size Dimension, concepts::NumericType CoordinateType>
const Size
CSGTraits<Dimension,CoordinateType>::dimension;


} // namespace cie::csg


// ---------------------------------------------------------
// CONCEPTS
// ---------------------------------------------------------
namespace cie::concepts {

namespace detail {

template <class T, class _ = void>
struct isCSGObject : std::false_type {};

template <class ...Args>
struct isCSGObjectHelper {};

template <class T>
struct isCSGObject<
    T,
    std::conditional_t<
        false,
        isCSGObjectHelper<
            typename T::coordinate_type,
            typename T::point_type
        >,
        void
    >
> : public std::true_type {};

} // namespace detail

template <class T>
concept CSGObject
= detail::isCSGObject<T>::value;

} // namespace cie::concepts


// ---------------------------------------------------------
// TRAIT GENERATORS
// ---------------------------------------------------------
namespace cie::csg {


namespace detail {

template <class T>
struct Traits
{
    using csg_traits = void;
};


template <concepts::CSGObject ObjectType>
struct Traits<ObjectType>
{
    using csg_traits = CSGTraits<ObjectType::dimension,typename ObjectType::coordinate_type>;
};


template <concepts::STLArray PointType>
struct Traits<PointType>
{
    using csg_traits = CSGTraits<std::tuple_size<PointType>::value,typename PointType::value_type>;
};


template <class T>
concept HasCSGTraits
= !std::is_same_v<
    typename Traits<T>::csg_traits,
    void
>;


} // namespace detail


/// Trait extractor
template <detail::HasCSGTraits ObjectType>
struct Traits : public detail::Traits<ObjectType>::csg_traits
{};


} // namespace cie::csg


#endif
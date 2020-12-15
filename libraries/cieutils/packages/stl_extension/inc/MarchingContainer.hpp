#ifndef CIE_UTILS_MARCHING_CONTAINER_HPP
#define CIE_UTILS_MARCHING_CONTAINER_HPP

// --- Internal Includes ---
#include "cieutils/packages/types/inc/types.hpp"
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- STL Includes ---
#include <vector>


namespace cie::utils {


/**
 * A container that only holds a specified number of last values
 */
template <class ValueType>
class MarchingContainer
{
public:
    using value_type = ValueType;
    using size_type  = Size;

public:
    MarchingContainer() = delete;
    MarchingContainer( const MarchingContainer<ValueType>& r_rhs ) = default;
    MarchingContainer<ValueType>& operator=( const MarchingContainer<ValueType>& r_rhs ) = default;

    /**
     * Construct a marching container of size identical to the size
     * of the argument container, and initialize it with its values.
     */
    template <class ContainerType>
    requires concepts::ClassContainer<ContainerType,ValueType>
    MarchingContainer( const ContainerType& r_initializer );

    /**
     * Construct a marching container of the specified size, initialized
     * with the specified initializer
     */
    MarchingContainer( Size capacity, const value_type& r_initializer );

    /**
     * Construct a marching container of the specified size,
     * with default initialized values
     */
    MarchingContainer( Size capacity );

    template <class ...Args>
    void emplace_back( Args&&... args );

    void push_back( const value_type& r_value );

    value_type& back();
    const value_type& back() const;

    value_type& operator[]( Size index );
    const value_type& operator[]( Size index ) const;

    /**
     * Return the number of times this container was pushed to
     * 
     * @note the number of values held will always be <capacity>
     */
    Size size() const;

private:
    bool outOfRangeCheck( Size index );

private:
    using container_type = std::vector<value_type>;

private:
    Size           _capacity;
    Size           _counter;
    container_type _container;
};


} // namespace cie::utils

#include "cieutils/packages/stl_extension/impl/MarchingContainer_impl.hpp"

#endif
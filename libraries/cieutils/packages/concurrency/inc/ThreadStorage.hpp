#ifndef CIE_UTILS_CONCURRENCY_THREAD_STORAGE_HPP
#define CIE_UTILS_CONCURRENCY_THREAD_STORAGE_HPP

// --- Internal Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"

// --- STL Includes ---
#include <tuple>
#include <functional>


namespace cie::mp {


template <concepts::Integer IndexType, class ...Args>
class ThreadStorage
{
public:
    using index_type        = IndexType;
    using storage_type      = std::tuple<Args...>;
    using reference_storage = std::tuple<Args&...>;
    using loop_function     = std::function<void(index_type,Args&...)>;
    template <class ValueType>
    using object_loop_function = std::function<void(ValueType&,Args&...)>;

public:
    ThreadStorage( Args&&... r_args ) :
        _values( std::forward<Args>(r_args)... )
    {}

    ThreadStorage( const ThreadStorage<IndexType,Args...>& r_rhs ) = default;
    ThreadStorage( ThreadStorage<IndexType,Args...>&& r_rhs ) = default;

    storage_type& values()                          { return _values; }
    const storage_type& values() const              { return _values; }

private:
    storage_type      _values;
};


} // namespace cie::mp


#endif
#ifndef CIE_UTILS_PARALLEL_FOR_HPP
#define CIE_UTILS_PARALLEL_FOR_HPP

// --- Internal Includes ---
#include "cieutils/packages/concurrency/inc/ThreadStorage.hpp"
#include "cieutils/packages/concurrency/inc/ThreadPool.hpp"
#include "cieutils/packages/concepts/inc/container_concepts.hpp"
#include "cieutils/packages/types/inc/types.hpp"

// --- STL Includes ---
#include <functional>
#include <thread>
#include <vector>


namespace cie::mp {


template < concepts::Integer IndexType = Size,
           class StorageType = ThreadStorage<IndexType> >
class ParallelFor
{
public:
    using index_partition = std::vector<IndexType>;

public:
    template <class ...Args>
    ParallelFor( Args&&... r_args );

    template <class ...Args>
    static ParallelFor<IndexType,ThreadStorage<IndexType,Args...>>
    firstPrivate( Args&&... r_args );

    ParallelFor<IndexType,StorageType>& setPool( ThreadPoolPtr p_pool );

    void operator()( IndexType indexMin,
                     IndexType indexMax,
                     IndexType stepSize,
                     typename StorageType::loop_function&& r_function );

    void operator()( IndexType indexMax,
                     typename StorageType::loop_function&& r_function );

    template <concepts::STLContainer ContainerType>
    void operator()( ContainerType& r_container,
                     typename StorageType::object_loop_function<typename ContainerType::value_type>&& r_function );

protected:
    virtual index_partition makeIndexPartition( IndexType indexMin,
                                                IndexType indexMax,
                                                IndexType stepSize );

    void execute( const index_partition& r_indexPartition,
                  IndexType stepSize,
                  const typename StorageType::loop_function& r_function );

    template <concepts::STLContainer ContainerType>
    void execute( const index_partition& r_indexPartition,
                  IndexType stepSize,
                  ContainerType& r_container,
                  typename StorageType::object_loop_function<typename ContainerType::value_type>&& r_function );

private:
    StorageType            _storage;
    ThreadPoolPtr          _p_pool;
};





namespace detail {

template < concepts::Integer IndexType,
           class StorageType >
void loopChunk( IndexType indexMin,
                IndexType indexMax,
                IndexType stepSize )
{

}

} // namespace detail





} // namespace cie::mp

#include "cieutils/packages/concurrency/impl/ParallelFor_impl.hpp"

#endif
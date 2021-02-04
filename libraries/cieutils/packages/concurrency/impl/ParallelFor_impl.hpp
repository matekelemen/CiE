#ifndef CIE_UTILS_PARALLEL_FOR_IMPL_HPP
#define CIE_UTILS_PARALLEL_FOR_IMPL_HPP

// --- Internal Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"


namespace cie::mp {


template < concepts::Integer IndexType,
           class StorageType >
template <class ...Args>
ParallelFor<IndexType,StorageType>::ParallelFor( Args&&... r_args ) :
    _storage( std::forward<Args>(r_args)... ),
    _p_pool( nullptr )
{
}


template < concepts::Integer IndexType,
           class StorageType >
template <class ...Args>
inline ParallelFor<IndexType,ThreadStorage<IndexType,Args...>>
ParallelFor<IndexType,StorageType>::firstPrivate( Args&&... r_args )
{
    CIE_BEGIN_EXCEPTION_TRACING

    return ParallelFor<IndexType,ThreadStorage<IndexType,Args...>>( std::forward<Args>(r_args)... );

    CIE_END_EXCEPTION_TRACING
}


template < concepts::Integer IndexType,
           class StorageType >
ParallelFor<IndexType,StorageType>&
ParallelFor<IndexType,StorageType>::setPool( ThreadPoolPtr p_pool )
{
    this->_p_pool = p_pool;
    return *this;
}


template < concepts::Integer IndexType,
           class StorageType >
inline void
ParallelFor<IndexType,StorageType>::operator()( IndexType indexMin,
                                                IndexType indexMax,
                                                IndexType stepSize,
                                                typename StorageType::loop_function&& r_function )
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->execute(
        std::move( this->makeIndexPartition(indexMin,indexMax,stepSize) ),
        stepSize,
        std::forward<typename StorageType::loop_function>(r_function)
    );

    CIE_END_EXCEPTION_TRACING
}


template < concepts::Integer IndexType,
           class StorageType >
inline void
ParallelFor<IndexType,StorageType>::operator()( IndexType indexMax,
                                                typename StorageType::loop_function&& r_function )
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->execute(
        std::move( this->makeIndexPartition(0, indexMax, 1) ),
        1,
        std::forward<typename StorageType::loop_function>(r_function)
    );

    CIE_END_EXCEPTION_TRACING
}


template < concepts::Integer IndexType,
           class StorageType >
template <concepts::STLContainer ContainerType>
inline void
ParallelFor<IndexType,StorageType>::operator()( ContainerType& r_container,
                                                typename StorageType::object_loop_function<typename ContainerType::value_type>&& r_function )
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->execute(
        std::move( this->makeIndexPartition(0,r_container.size(),1) ),
        1,
        r_container,
        std::forward<typename StorageType::object_loop_function<typename ContainerType::value_type>>( r_function )
    );

    CIE_END_EXCEPTION_TRACING
}


template < concepts::Integer IndexType,
           class StorageType >
typename ParallelFor<IndexType,StorageType>::index_partition
ParallelFor<IndexType,StorageType>::makeIndexPartition( IndexType indexMin,
                                                        IndexType indexMax,
                                                        IndexType stepSize )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Create a pool if one wasn't provided already
    if ( !this->_p_pool )
        this->_p_pool.reset( new ThreadPool );

    // Check index range
    CIE_OUT_OF_RANGE_CHECK( stepSize != 0 )
    CIE_OUT_OF_RANGE_CHECK( (indexMin <= indexMax) == (0 < stepSize) );

    // Handle reverse loops
    if ( stepSize < 0 )
    {
        auto tmp = indexMin;
        indexMin = indexMax;
        indexMax = tmp;
        stepSize = 0 - stepSize;
    }

    typename ParallelFor<IndexType,StorageType>::index_partition indexPartition;

    // Initialize index blocks
    IndexType blockSize = stepSize * (indexMax - indexMin) / (stepSize * this->_p_pool->size() + 1);

    if ( blockSize == 0 )
        blockSize = 1;

    IndexType tmp = indexMin;
    
    indexPartition.reserve( this->_p_pool->size() + 1 );
    for ( IndexType i=0; i<this->_p_pool->size() && tmp<indexMax; ++i, tmp+=blockSize )
        indexPartition.push_back( tmp );
    
    indexPartition.push_back( indexMax );

    return indexPartition;

    CIE_END_EXCEPTION_TRACING
}


template < concepts::Integer IndexType,
           class StorageType >
void
ParallelFor<IndexType,StorageType>::execute( const typename ParallelFor<IndexType,StorageType>::index_partition& r_indexPartition,
                                             IndexType stepSize,
                                             const typename StorageType::loop_function& r_function )
{
    CIE_BEGIN_EXCEPTION_TRACING

    CIE_OUT_OF_RANGE_CHECK( r_indexPartition.size() <= this->_p_pool->size() + 1 )

    // Schedule jobs
    for ( IndexType parititonIndex=0; parititonIndex<r_indexPartition.size()-1; ++parititonIndex )
    {
        this->_p_pool->queueJob(
            [this, parititonIndex, stepSize, r_indexPartition, r_function]() -> void
            {
                typename StorageType::storage_type storage = this->_storage.values();
                typename StorageType::reference_storage storageReference = std::make_from_tuple<typename StorageType::reference_storage>( storage );

                for ( IndexType i=r_indexPartition[parititonIndex]; i<r_indexPartition[parititonIndex+1]; i+=stepSize )
                {
                    std::apply(
                        [i,&r_function]( auto&... args ) -> void { r_function(i,args...); },
                        storageReference
                    );
                }
            }
        );
    }

    CIE_END_EXCEPTION_TRACING
}


template < concepts::Integer IndexType,
           class StorageType >
template <concepts::STLContainer ContainerType>
void
ParallelFor<IndexType,StorageType>::execute( const typename ParallelFor<IndexType,StorageType>::index_partition& r_indexPartition,
                                             IndexType stepSize,
                                             ContainerType& r_container,
                                             typename StorageType::object_loop_function<typename ContainerType::value_type>&& r_function )
{
    CIE_BEGIN_EXCEPTION_TRACING

    CIE_OUT_OF_RANGE_CHECK( r_indexPartition.size() <= this->_p_pool->size() + 1 )

    // Schedule jobs
    for ( IndexType parititonIndex=0; parititonIndex<r_indexPartition.size()-1; ++parititonIndex )
    {
        this->_p_pool->queueJob(
            [this, parititonIndex, stepSize, r_indexPartition, r_function, &r_container]() -> void
            {
                typename StorageType::storage_type storage = this->_storage.values();
                typename StorageType::reference_storage storageReference = std::make_from_tuple<typename StorageType::reference_storage>( storage );

                for ( IndexType i=r_indexPartition[parititonIndex]; i<r_indexPartition[parititonIndex+1]; i+=stepSize )
                {
                    std::apply(
                        [i,&r_function,&r_container]( auto&... args ) -> void { r_function(r_container[i],args...); },
                        storageReference
                    );
                }
            }
        );
    }

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::mp


#endif
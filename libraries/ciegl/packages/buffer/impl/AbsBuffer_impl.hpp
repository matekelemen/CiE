#ifndef CIE_GL_ABS_BUFFER_IMPL_HPP
#define CIE_GL_ABS_BUFFER_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/stl_extension/inc/resize.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- STL Includes ---
#include <algorithm>


namespace cie::gl {


template <class DataType, class DataContainer>
AbsBuffer<DataType,DataContainer>::AbsBuffer() :
    utils::IDObject<Size>()
{
}


template <class DataType, class DataContainer>
AbsBuffer<DataType,DataContainer>::AbsBuffer( Size id ) :
    utils::IDObject<Size>(id)
{
}


template <class DataType, class DataContainer>
template <class ContainerType>
requires concepts::detail::ClassContainerWithException<ContainerType,DataType,typename AbsBuffer<DataType>::data_container_type>
inline void
AbsBuffer<DataType,DataContainer>::write( const ContainerType& r_dataContainer )
{
    CIE_BEGIN_EXCEPTION_TRACING

    typename AbsBuffer<DataType>::data_container_type dataContainer;
    utils::resize( dataContainer, r_dataContainer.size() );
    std::copy(  r_dataContainer.begin(),
                r_dataContainer.end(),
                dataContainer.begin() );
    this->write( dataContainer );

    CIE_END_EXCEPTION_TRACING
}


template <class DataType, class DataContainer>
template <class ContainerType>
requires concepts::detail::ClassContainerWithException<ContainerType,DataType,typename AbsBuffer<DataType>::data_container_type>
inline void
AbsBuffer<DataType,DataContainer>::write( Size begin, const ContainerType& r_dataContainer )
{
    CIE_BEGIN_EXCEPTION_TRACING

    typename AbsBuffer<DataType>::data_container_type dataContainer;
    utils::resize( dataContainer, r_dataContainer.size() );
    std::copy(  r_dataContainer.begin(),
                r_dataContainer.end(),
                dataContainer.begin() );
    this->write( begin, dataContainer );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::gl

#endif
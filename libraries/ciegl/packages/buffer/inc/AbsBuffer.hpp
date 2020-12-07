#ifndef CIE_GL_ABS_BUFFER_HPP
#define CIE_GL_ABS_BUFFER_HPP

// --- External Includes ---
#include "glad/glad.h"

// --- Utility Includes ---
#include "cieutils/packages/types/inc/IDObject.hpp"
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- Internal Includes ---
#include "ciegl/packages/buffer/inc/AttributeContainer.hpp"

// --- STL Includes ---
#include <memory>
#include <vector>


// Avoid compiler weirdness
namespace cie::concepts::detail {
template <class ContainerType1,class ValueType,class ContainerType2>
concept ClassContainerWithException
= ClassContainer<ContainerType1,ValueType>
    && !std::is_same_v<ContainerType1,ContainerType2>;
} // namespace cie::concepts::detail


namespace cie::gl {


template <class DataType, class DataContainer = std::vector<DataType>>
requires concepts::ClassContainer<DataContainer,DataType>
class AbsBuffer : public utils::IDObject<Size>
{
public:
    using data_type             = DataType;
    using data_container_type   = DataContainer;   // <-- contiguous data container

public:
    friend class AbsBufferManager;

public:
    AbsBuffer();
    AbsBuffer( Size id );

//protected:

    /**
     * Allocate the requested number of bytes
     * (uninitialized data)
     */
    virtual void reserve( Size byteCount ) = 0;

    /**
     * Resize and write data to the GPU buffer. Only a buffer manager may issue
     * a write request, since it's the only entity that keeps track
     * of bound buffers.
    */
    virtual void write( const data_container_type& r_dataContainer ) = 0;

    /**
     * Write data to an existing buffer
     */
    virtual void write( Size begin, const data_container_type& r_dataContainer ) = 0;

    /**
     * Copy data to a contiguous internal container, then write
     * it to the resized GPU buffer.
    */
    template <class ContainerType>
    requires concepts::detail::ClassContainerWithException<ContainerType,DataType,typename AbsBuffer<DataType>::data_container_type>
    void write( const ContainerType& r_dataContainer );

    /**
     * Copy data to a contiguous internal container, then write
     * it to the existing GPU buffer.
     */
    template <class ContainerType>
    requires concepts::detail::ClassContainerWithException<ContainerType,DataType,typename AbsBuffer<DataType>::data_container_type>
    void write( Size begin, const ContainerType& r_dataContainer );
};


using VertexBuffer       = AbsBuffer<GLfloat,AttributeContainer>;
using ElementBuffer      = AbsBuffer<GLuint>;

using VertexBufferPtr    = std::shared_ptr<VertexBuffer>;
using ElementBufferPtr   = std::shared_ptr<ElementBuffer>;


} // namespace cie::gl

#include "ciegl/packages/buffer/impl/AbsBuffer_impl.hpp"

#endif
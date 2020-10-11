#ifndef CIE_GL_ABS_BUFFER_MANAGER_HPP
#define CIE_GL_ABS_BUFFER_MANAGER_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- Internal Includes ---
#include "ciegl/packages/buffer/inc/AbsBuffer.hpp"
#include "ciegl/packages/context/inc/AbsContextClass.hpp"

// --- STL Includes ---
#include <list>


namespace cie::gl {


class AbsBufferManager //: public AbsContextClass
{
public:
    using vertex_buffer_container   = std::list<VertexBufferPtr>;
    using element_buffer_container  = std::list<ElementBufferPtr>;

public:
    //AbsBufferManager(  )

    /**
     * Create new vertex buffer
     * and register it in the internal container.
    */
    [[nodiscard]] VertexBufferPtr makeVertexBuffer();

    /**
     * Create new element buffer
     * and register it in the internal container.
    */
    [[nodiscard]] ElementBufferPtr makeElementBuffer();

    /**
     * Bind vertex buffer and set bound flag.
    */
    void bindVertexBuffer( VertexBufferPtr p_vertexBuffer );

    /**
     * Bind vertex buffer and set bound flag.
    */
    void bindElementBuffer( ElementBufferPtr p_elementBuffer );


    const vertex_buffer_container& vertexBuffers() const;
    const element_buffer_container& elementBuffers() const;

protected:
    template <class BufferPtr, class BufferContainer>
    requires concepts::ClassContainer<BufferContainer,BufferPtr>
    void registerBuffer( BufferPtr p_buffer, BufferContainer& r_bufferContainer );
    
    virtual VertexBufferPtr makeVertexBuffer_impl() = 0;
    virtual ElementBufferPtr makeElementBuffer_impl() = 0;
    virtual void bindVertexBuffer_impl( VertexBufferPtr p_vertexBuffer ) = 0;
    virtual void bindElementBuffer_impl( ElementBufferPtr p_elementBuffer ) = 0;

protected:
    VertexBufferPtr          _p_boundVertexBuffer;
    ElementBufferPtr         _p_boundElementBuffer;

    vertex_buffer_container     _vertexBuffers;
    element_buffer_container    _elementBuffers;
};


} // namespace cie::gl

#include "ciegl/packages/buffer/impl/AbsBufferManager_impl.hpp"

#endif
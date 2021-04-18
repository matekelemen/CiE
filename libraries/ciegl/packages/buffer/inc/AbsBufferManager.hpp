#ifndef CIE_GL_ABS_BUFFER_MANAGER_HPP
#define CIE_GL_ABS_BUFFER_MANAGER_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"
#include "cieutils/packages/logging/inc/Loggee.hpp"
#include "cieutils/packages/logging/inc/LoggerSingleton.hpp"

// --- Internal Includes ---
#include "ciegl/packages/buffer/inc/AbsBuffer.hpp"

// --- STL Includes ---
#include <list>
#include <memory>


namespace cie::gl {


class AbsBufferManager : public utils::Loggee
{
public:
    using vertex_buffer_container   = std::list<VertexBufferPtr>;
    using element_buffer_container  = std::list<ElementBufferPtr>;

public:
    AbsBufferManager( const std::string& r_name = "AbsBufferManager",
                      utils::Logger& r_logger = utils::LoggerSingleton::get() );

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
     * Register an existing vertex buffer
     */
    AbsBufferManager& addBuffer( VertexBufferPtr p_buffer );

    /**
     * Register an existing element buffer
     */
    AbsBufferManager& addBuffer( ElementBufferPtr p_buffer );

    /**
     * Bind vertex buffer and set bound flag.
    */
    AbsBufferManager& bindVertexBuffer( VertexBufferPtr p_vertexBuffer,
                                        bool log = true );

    /**
     * Bind vertex buffer and set bound flag.
    */
    AbsBufferManager& bindElementBuffer( ElementBufferPtr p_elementBuffer,
                                         bool log = true );

    /**
     * Check whether a vertex buffer is bound
     */
    bool hasBoundVertexBuffer() const;

    /**
     * Check whether an element buffer is bound
     */
    bool hasBoundElementBuffer() const;

    /**
     * Write to the bound vertex buffer
     */
    template <class ContainerType>
    AbsBufferManager& writeToBoundVertexBuffer( const ContainerType& r_data );

    template <class ContainerType>
    AbsBufferManager& writeToBoundVertexBuffer( Size begin,
                                                const ContainerType& r_data );

    /**
     * Write to the bound element buffer
     */
    template <class ContainerType>
    AbsBufferManager& writeToBoundElementBuffer( const ContainerType& r_data );

    template <class ContainerType>
    AbsBufferManager& writeToBoundElementBuffer( Size begin,
                                                 const ContainerType& r_data );

    const VertexBufferPtr& boundVertexBuffer() const;
    const ElementBufferPtr& boundElementBuffer() const;

    const vertex_buffer_container& vertexBuffers() const;
    const element_buffer_container& elementBuffers() const;

protected:    
    virtual VertexBufferPtr makeVertexBuffer_impl() = 0;
    virtual ElementBufferPtr makeElementBuffer_impl() = 0;
    virtual void bindVertexBuffer_impl( VertexBufferPtr p_vertexBuffer ) = 0;
    virtual void bindElementBuffer_impl( ElementBufferPtr p_elementBuffer ) = 0;

private:
    template <class BufferPtr, class BufferContainer>
    requires concepts::ClassContainer<BufferContainer,BufferPtr>
    void registerBuffer( BufferPtr p_buffer, BufferContainer& r_bufferContainer );

protected:
    VertexBufferPtr          _p_boundVertexBuffer;
    ElementBufferPtr         _p_boundElementBuffer;

    vertex_buffer_container     _vertexBuffers;
    element_buffer_container    _elementBuffers;
};


using BufferManagerPtr      = std::shared_ptr<AbsBufferManager>;
using BufferManagerConstPtr = std::shared_ptr<const AbsBufferManager>;


} // namespace cie::gl

#include "ciegl/packages/buffer/impl/AbsBufferManager_impl.hpp"

#endif
#ifndef CIE_GL_SHAPES_GL_SHAPE_HPP
#define CIE_GL_SHAPES_GL_SHAPE_HPP

// --- Internal Includes ---
#include "ciegl/packages/shapes/inc/AbsGLShape.hpp"
#include "ciegl/packages/buffer/inc/AbsVertex.hpp"


namespace cie::gl {


template <concepts::GLVertex VertexType>
class GLShape : public AbsGLShape
{
public:
    using vertex_type      = VertexType;
    using vertex_ptr       = std::shared_ptr<vertex_type>;
    using vertex_container = std::vector<vertex_ptr>;

    using AbsGLShape::attribute_type;
    using AbsGLShape::attribute_container;
    using AbsGLShape::attribute_container_ptr;
    using AbsGLShape::index_type;
    using AbsGLShape::index_container;

public:
    GLShape( attribute_container_ptr p_attributes );
    
    GLShape( GLShape<VertexType>&& r_rhs ) = default;

    void setAttribute( Size attributeIndex,
                       Size componentIndex,
                       attribute_type value ) override;

protected:
    GLShape() = delete;
    GLShape( const GLShape<VertexType>& r_rhs ) = delete;
    GLShape<VertexType>& operator=( const GLShape<VertexType>& r_rhs ) = delete;

protected:
    vertex_container _vertices;
};


} // namespace cie::gl

#include "ciegl/packages/shapes/impl/GLShape_impl.hpp"

#endif
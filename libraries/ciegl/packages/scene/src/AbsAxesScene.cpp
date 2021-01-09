// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/scene/inc/AbsAxesScene.hpp"

/*
namespace cie::gl {


AbsAxesScene::AbsAxesScene( utils::Logger& r_logger,
                            const std::string& r_name,
                            ShaderPtr p_vertexShader,
                            ShaderPtr p_geometryShader,
                            ShaderPtr p_fragmentShader,
                            VertexBufferPtr p_vertexBuffer,
                            ElementBufferPtr p_elementBuffer ) :
    Scene( r_logger,
           r_name,
           p_vertexShader,
           p_geometryShader,
           p_fragmentShader,
           p_vertexBuffer,
           p_elementBuffer ),
    _p_attributeContainer( new AttributeContainer ),
    _vertices(),
    _scale( { 1.0, 1.0, 1.0 } ),
    _axesRequireUpdate( true )
{
    CIE_BEGIN_EXCEPTION_TRACING

    using vertex_type = AbsAxesScene::vertex_type;

    // x
    this->_vertices.push_back(
        makeVertex<vertex_type>( this->_p_attributeContainer,
                             { 0.0, 0.0, 0.0 },
                             { 1.0, 0.0, 0.0, 1.0 } );
    );

    this->_vertices.push_back(
        makeVertex<vertex_type>( this->_p_attributeContainer,
                             { 1.0, 0.0, 0.0 },
                             { 1.0, 0.0, 0.0, 1.0 } );
    );

    // y
    this->_vertices.push_back(
        makeVertex<vertex_type>( this->_p_attributeContainer,
                             { 0.0, 0.0, 0.0 },
                             { 0.0, 1.0, 0.0, 1.0 } );
    );

    this->_vertices.push_back(
        makeVertex<vertex_type>( this->_p_attributeContainer,
                             { 0.0, 1.0, 0.0 },
                             { 0.0, 1.0, 0.0, 1.0 } );
    );

    // z
    this->_vertices.push_back(
        makeVertex<vertex_type>( this->_p_attributeContainer,
                             { 0.0, 0.0, 0.0 },
                             { 0.0, 0.0, 1.0, 1.0 } );
    );

    this->_vertices.push_back(
        makeVertex<vertex_type>( this->_p_attributeContainer,
                             { 0.0, 0.0, 1.0 },
                             { 0.0, 0.0, 1.0, 1.0 } );
    );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::gl
*/
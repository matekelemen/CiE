// --- Graphics Includes ---
#include <ciegl/ciegl.hpp>

// --- Utility Includes ---
#include <cieutils/macros.hpp>

// --- Internal Includes ---
#include "ViewerScene.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <numeric>
#include <algorithm>


namespace cie {


const std::string SHADER_DIR = SOURCE_PATH + "/applications/3DViewer/data/shaders";


ViewerScene::ViewerScene( utils::Logger& r_logger ) :
    gl::Scene(
        r_logger,
        "3dViewerScene",
        gl::makeVertexShader<gl::GLFWVertexShader>( SHADER_DIR + "/vertexShader.xml",
                                                    SHADER_DIR + "/vertexShader.glsl" ),
        gl::makeGeometryShader<gl::GLFWGeometryShader>( SHADER_DIR + "/geometryShader.xml",
                                                        SHADER_DIR + "/geometryShader.glsl" ),
        gl::makeFragmentShader<gl::GLFWFragmentShader>( SHADER_DIR + "/fragmentShader.xml",
                                                        SHADER_DIR + "/fragmentShader.glsl" )
    ),
    _models(),
    _updateModels(true)
{
    glEnable( GL_DEPTH_TEST );

    auto p_camera = this->makeCamera<gl::Camera<gl::PerspectiveProjection>>();
    this->bindUniform( "transformation", p_camera->transformationMatrix() );
    this->bindUniform( "cameraPosition", p_camera->position() );
}


void ViewerScene::addModel( gl::PartPtr p_model )
{
    this->_models.push_back( p_model );
    this->_updateModels = true;
}


void ViewerScene::removeModel( gl::PartPtr p_model )
{
    auto it = std::find(
        this->_models.begin(),
        this->_models.end(),
        p_model
    );

    if ( it != this->_models.end() )
    {
        this->_models.erase( it );
        this->_updateModels = true;
    }
}


gl::CameraPtr ViewerScene::getCamera()
{
    return *this->_cameras.begin();
}


void ViewerScene::updateModels()
{
    // Allocate memory on the GPU
    Size byteCount     = 0;
    Size triangleCount = 0;
    for ( const auto& rp_model : this->_models )
    {
        byteCount     += rp_model->byteCount();
        triangleCount += rp_model->numberOfPrimitives();
    }

    this->_p_bufferManager->boundVertexBuffer()->reserve( byteCount );

    // Write indices to the element buffer
    {
        typename gl::GLFWElementBuffer::data_container_type triangles( 3 * triangleCount );
        std::iota( triangles.begin(),
                   triangles.end(),
                   0 );
        this->_p_bufferManager->writeToBoundElementBuffer( triangles );
    }

    // Write attributes to the array buffer
    Size attributeOffset = 0;

    for ( const auto& rp_model : this->_models )
    {
        this->_p_bufferManager->writeToBoundVertexBuffer(
            attributeOffset,
            rp_model->data()
        );

        attributeOffset += rp_model->byteCount();
    }

    this->_updateModels = false;
}


void ViewerScene::update_impl()
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( this->_updateModels )
        this->updateModels();

    GLint64 numberOfIndices;
    glGetBufferParameteri64v( GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &numberOfIndices );
    numberOfIndices /= sizeof( GLuint );
    glDrawElements( GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0 );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie
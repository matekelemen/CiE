// --- Graphics Includes ---
#include <ciegl/ciegl.hpp>

// --- Utility Includes ---
#include <cieutils/macros.hpp>
#include <cieutils/ranges.hpp>

// --- Internal Includes ---
#include "ViewerScene.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <numeric>
#include <algorithm>
#include <filesystem>
#include <ranges>


namespace cie {


const std::filesystem::path SHADER_DIR = SOURCE_PATH / "applications/3DViewer/data/shaders";


ViewerScene::ViewerScene( utils::Logger& r_logger ) :
    gl::Scene(
        r_logger,
        "3dViewerScene",
        gl::makeVertexShader<gl::GLFWVertexShader>( SHADER_DIR / "vertexShader.xml",
                                                    SHADER_DIR / "vertexShader.glsl" ),
        gl::makeGeometryShader<gl::GLFWGeometryShader>( SHADER_DIR / "geometryShader.xml",
                                                        SHADER_DIR / "geometryShader.glsl" ),
        gl::makeFragmentShader<gl::GLFWFragmentShader>( SHADER_DIR / "fragmentShader.xml",
                                                        SHADER_DIR / "fragmentShader.glsl" )
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
    Size attributeByteCount = 0;
    Size indexByteCount     = 0;
    Size triangleCount      = 0;
    for ( const auto& rp_model : this->_models )
    {
        attributeByteCount += rp_model->attributeByteCount();
        indexByteCount     += rp_model->indexByteCount();
        triangleCount      += rp_model->numberOfPrimitives();
    }

    this->_p_bufferManager->boundVertexBuffer()->reserve( attributeByteCount );
    this->_p_bufferManager->boundElementBuffer()->reserve( indexByteCount );

    // Write to buffers
    Size attributeByteOffset = 0;
    Size indexByteOffset     = 0;
    Size indexOffset         = 0;

    for ( const auto& rp_model : this->_models )
    {
        this->_p_bufferManager->writeToBoundVertexBuffer(
            attributeByteOffset,
            rp_model->data()
        );

        // Offset vertex indices
        auto offsetIndexRange = utils::makeTransformView<gl::Part::index_type>(
            rp_model->indices(),
            [indexOffset](auto index) { return index + indexOffset; }
        );

        this->_p_bufferManager->writeToBoundElementBuffer(
            indexByteOffset,
            offsetIndexRange
        );

        attributeByteOffset += rp_model->attributeByteCount();
        indexByteOffset     += rp_model->indexByteCount();
        indexOffset         = rp_model->numberOfVertices();
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
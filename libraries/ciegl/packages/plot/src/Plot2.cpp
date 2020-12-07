// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/plot/inc/Plot2.hpp"
#include "ciegl/packages/shaders/inc/GLFWShader.hpp"

#include "ciegl/packages/camera/inc/Camera.hpp"
#include "ciegl/packages/camera/inc/OrthographicProjection.hpp"

#include "ciegl/packages/utility/inc/GLError.hpp"


namespace cie::gl {


/* --- Plot2 --- */

Plot2::Plot2( WindowPtr p_window ) :
    AbsPlot( p_window )
{
    this->initializeScene();
}


Plot2::Plot2() :
    AbsPlot()
{
    this->initializeScene();
}


void Plot2::initializeScene()
{
    CIE_BEGIN_EXCEPTION_TRACING

    std::cin.get();
    this->_p_scene = this->_p_window->makeScene<Plot2::Plot2Scene>( "Plot2Scene",
                                                                    this->_p_attributes );
    
    using CameraType = Camera<OrthographicProjection>;
    this->_p_camera = this->_p_scene->makeCamera<CameraType>();
    this->_p_scene->bindUniform( "transformation", this->_p_camera->transformationMatrix() );

    CIE_END_EXCEPTION_TRACING
}


/* --- Plot2::Plot2Scene --- */

const std::filesystem::path CURRENT_SHADER_DIR = SOURCE_PATH / "libraries/ciegl/data/shaders/plot2";


Plot2::Plot2Scene::Plot2Scene( utils::Logger& r_logger,
                               const std::string& r_name,
                               AttributeContainerPtr p_attributes ) :
    Scene(
        r_logger,
        r_name,
        makeVertexShader<GLFWVertexShader>( CURRENT_SHADER_DIR / "vertexShader.xml",
                                            CURRENT_SHADER_DIR / "vertexShader.glsl" ),
        makeGeometryShader<GLFWGeometryShader>( CURRENT_SHADER_DIR / "geometryShader.xml",
                                                CURRENT_SHADER_DIR / "geometryShader.glsl" ),
        makeFragmentShader<GLFWFragmentShader>( CURRENT_SHADER_DIR / "fragmentShader.xml",
                                                CURRENT_SHADER_DIR / "fragmentShader.glsl" )
    ),
    _p_attributes( p_attributes ),
    _updateFlag( true )
{
}


CameraPtr Plot2::Plot2Scene::getCamera()
{
    return *this->_cameras.begin();
}


void Plot2::Plot2Scene::setUpdateFlag()
{
    this->_updateFlag = true;
}


void Plot2::Plot2Scene::update_impl()
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Write attributes into the buffer if requested
    if ( this->_updateFlag )
    {
        std::cout << this->_p_attributes->size() << std::endl;

        this->_p_bufferManager->writeToBoundVertexBuffer( *this->_p_attributes );
        this->_updateFlag = false;
    }

    checkGLErrors( *this, "Error writing to buffer" );

    GLint64 numberOfAttributes;
    glGetBufferParameteri64v( GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &numberOfAttributes );
    numberOfAttributes /= sizeof( GLfloat );

    glDrawArrays( GL_LINE_STRIP, 0, numberOfAttributes );

    CIE_END_EXCEPTION_TRACING
}




} // namespace cie::gl
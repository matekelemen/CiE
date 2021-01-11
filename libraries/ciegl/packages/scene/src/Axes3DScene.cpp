// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/ranges/inc/TransformView.hpp"

// --- Internal Includes ---
#include "ciegl/packages/scene/inc/Axes3DScene.hpp"
#include "ciegl/packages/file/inc/GenericPart.hpp"
#include "cmake_variables.hpp"


namespace cie::gl {


Axes3DScene::Axes3DScene( utils::Logger& r_logger,
                          const std::string& r_name,
                          CameraPtr p_camera ) :
    GenericPartScene( r_logger,
                      r_name,
                      GL_TRIANGLES,
                      SOURCE_PATH / "libraries/ciegl/data/shaders/coloredAxes" ),
    _arrows()
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Set camera and bind uniforms
    this->addCamera( p_camera );
    this->bindUniform( "transformation", p_camera->transformationMatrix() );

    // Get number of vertex attributes
    Size vertexAttributeSize = makeVertex<ColoredVertex3>(
        ColoredVertex3::attribute_container_ptr( new ColoredVertex3::attribute_container )
    )->numberOfAttributes();

    auto p_part = GenericPartPtr(
        new GenericPart( 3, vertexAttributeSize, 3 )
    );

    auto insertIndices = [p_part]( const GenericPart::index_container& r_indices, Size indexOffset )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        auto view = utils::makeTransformView<GenericPart::index_container::value_type>(
            r_indices,
            [indexOffset]( auto index ) { return index + indexOffset; }  
        );

        p_part->indices().reserve( p_part->indices().size() + r_indices.size() );

        p_part->indices().insert( p_part->indices().begin(),
                                  view.begin(),
                                  view.end() );

        CIE_END_EXCEPTION_TRACING
    };

    Size indexOffset = 0;
    Axes3DScene::arrow_type* p_arrow;

    // x axis (red)
    this->_arrows.emplace_back( Axes3DScene::arrow_type(
        p_part->attributes(),
        {0.0, 0.0, 0.0},
        {1.0, 0.0, 0.0}
    ));

    p_arrow = &this->_arrows.back();

    p_arrow->setAttribute( 1, 1, 0.0 ); // green component
    p_arrow->setAttribute( 1, 2, 0.0 ); // blue component

    insertIndices( p_arrow->indices(),
                   indexOffset );

    indexOffset += p_arrow->vertices().size();

    // y axis (green)
    this->_arrows.emplace_back( Axes3DScene::arrow_type(
        p_part->attributes(),
        {0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0}
    ));

    p_arrow = &this->_arrows.back();

    p_arrow->setAttribute( 1, 0, 0.0 ); // red component
    p_arrow->setAttribute( 1, 2, 0.0 ); // blue component

    insertIndices( p_arrow->indices(),
                   indexOffset );

    indexOffset += p_arrow->vertices().size();

    // z axis (blue)
    this->_arrows.emplace_back( Axes3DScene::arrow_type(
        p_part->attributes(),
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 1.0}
    ));

    p_arrow = &this->_arrows.back();

    p_arrow->setAttribute( 1, 0, 0.0 ); // red component
    p_arrow->setAttribute( 1, 1, 0.0 ); // green component

    insertIndices( p_arrow->indices(),
                   indexOffset );

    indexOffset += p_arrow->vertices().size();

    // Add the part containing the arrows
    this->addPart( p_part );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::gl
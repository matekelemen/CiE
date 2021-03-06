// --- Utility Includes ---
#include "cieutils/packages/ranges/inc/TransformView.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Incldues ---
#include "ciegl/packages/scene/inc/PartScene.hpp"

// --- STL Includes ---
#include <algorithm>


namespace cie::gl {


PartScene::PartScene( const std::string& r_name,
                      PartScene::part_container&& r_parts,
                      ShaderPtr p_vertexShader,
                      ShaderPtr p_geometryShader,
                      ShaderPtr p_fragmentShader,
                      utils::Logger& r_logger,
                      VertexBufferPtr p_vertexBuffer,
                      ElementBufferPtr p_elementBuffer ) :
    Scene( r_name,
           p_vertexShader,
           p_geometryShader,
           p_fragmentShader,
           r_logger,
           p_vertexBuffer,
           p_elementBuffer ),
    _parts( r_parts ),
    _partsRequireUpdate( true )
{
    CIE_BEGIN_EXCEPTION_TRACING

    glEnable( GL_DEPTH_TEST );

    CIE_END_EXCEPTION_TRACING
}


void PartScene::addPart( PartPtr p_part )
{
    this->_parts.push_back( p_part );
    this->_partsRequireUpdate = true;
}


void PartScene::removePart( PartPtr p_part )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto itp_part = std::find( this->_parts.begin(),
                               this->_parts.end(),
                               p_part );
    
    if ( itp_part != this->_parts.end() )
    {
        this->_parts.erase( itp_part );
        this->_partsRequireUpdate = true;
    }
    else
        CIE_THROW( Exception, "Could not find specified part in container" )

    CIE_END_EXCEPTION_TRACING
}


void PartScene::updateParts()
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Allocate memory on the GPU
    Size attributeByteCount = 0;
    Size indexByteCount     = 0;
    Size triangleCount      = 0;
    for ( const auto& rp_part : this->_parts )
    {
        attributeByteCount += rp_part->attributeByteCount();
        indexByteCount     += rp_part->indexByteCount();
        triangleCount      += rp_part->numberOfPrimitives();
    }

    this->_p_bufferManager->boundVertexBuffer()->reserve( attributeByteCount );
    this->_p_bufferManager->boundElementBuffer()->reserve( indexByteCount );

    // Write to buffers
    Size attributeByteOffset = 0;
    Size indexByteOffset     = 0;
    Size indexOffset         = 0;

    for ( const auto& rp_part : this->_parts )
    {
        this->_p_bufferManager->writeToBoundVertexBuffer(
            attributeByteOffset,
            *rp_part->attributes()
        );

        // Offset vertex indices (without modifying the original ones)
        auto offsetIndexRange = utils::makeTransformView<Part::index_type>(
            rp_part->indices(),
            [indexOffset](auto index) { return index + indexOffset; }
        );

        this->_p_bufferManager->writeToBoundElementBuffer(
            indexByteOffset,
            offsetIndexRange
        );

        attributeByteOffset += rp_part->attributeByteCount();
        indexByteOffset     += rp_part->indexByteCount();
        indexOffset         += rp_part->numberOfVertices();
    }

    this->_partsRequireUpdate = false;

    CIE_END_EXCEPTION_TRACING
}





} // namespace cie::gl
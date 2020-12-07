// --- Utility Includes ---
#include "cieutils/packages/macros/inc/checks.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"

// --- Internal Includes ---
#include "ciegl/packages/buffer/inc/AbsVertex.hpp"


namespace cie::gl {


AbsVertex::AbsVertex( AttributeContainerPtr p_attributes,
                      Size offset,
                      const AbsVertex::offset_container& r_attributeOffsets ) :
    _p_attributes( p_attributes ),
    _offset( offset ),
    _r_attributeOffsets( r_attributeOffsets )
{
    CIE_CHECK_POINTER( this->_p_attributes )
}


void AbsVertex::resizeAttributeContainer()
{
    utils::resize( *this->_p_attributes, this->_offset + this->numberOfAttributes() );
}


} // namespace cie::gl
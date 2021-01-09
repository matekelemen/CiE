#ifndef CIE_GL_GL_TRAITS_HPP
#define CIE_GL_GL_TRAITS_HPP

// --- External Includes ---
#include <glad/glad.h>

// --- Internal Includes ---
#include "ciegl/packages/buffer/inc/AttributeContainer.hpp"

// --- STL Includes ---
#include <array>
#include <vector>


namespace cie::gl {


struct GLTraits
{
    using attribute_type          = AttributeContainer::value_type;
    using attribute_container     = AttributeContainer;
    using attribute_container_ptr = AttributeContainerPtr;

    using index_type      = GLuint;
    using index_container = std::vector<index_type>;

    using color_type  = std::array<attribute_type,4>;
};


} // namespace cie::gl


#endif
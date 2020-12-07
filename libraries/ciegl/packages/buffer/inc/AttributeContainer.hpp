#ifndef CIE_GL_BUFFER_ATTRIBUTE_CONTAINER_HPP
#define CIE_GL_BUFFER_ATTRIBUTE_CONTAINER_HPP

// --- External Includes ---
#include "glad/glad.h"

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"

// --- STL Includes ---
#include <vector>
#include <memory>
#include <concepts>


namespace cie::gl {


class AbsVertex;
using VertexPtr = std::shared_ptr<AbsVertex>;

class AttributeContainer;
using AttributeContainerPtr = std::shared_ptr<AttributeContainer>;



class AttributeContainer :
    public std::vector<GLfloat>,
    public std::enable_shared_from_this<AttributeContainer>
{
public:
    using base_container = std::vector<GLfloat>;

public:
    AttributeContainer( const std::initializer_list<typename AttributeContainer::value_type>& r_list );

    template <class ...Args>
    AttributeContainer( Args&&... args );
};


} // namespace cie::gl

#include "ciegl/packages/buffer/impl/AttributeContainer_impl.hpp"

#endif
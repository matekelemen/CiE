#ifndef CIE_GL_ABS_VERTEX_HPP
#define CIE_GL_ABS_VERTEX_HPP

// --- External Includes ---
#include "glad/glad.h"

// --- Utility Includes ---
#include "cieutils/packages/types/inc/types.hpp"

// --- Internal Includes ---
#include "ciegl/packages/buffer/inc/AttributeContainer.hpp"

// --- STL Includes ---
#include <vector>
#include <memory>
#include <ranges>


namespace cie::gl {


class AbsVertex
{
public:
    using value_type           = typename AttributeContainer::value_type;
    using offset_container     = std::vector<Size>;

    using attribute_range       = std::ranges::subrange<typename AttributeContainer::iterator>;
    using attribute_const_range = std::ranges::subrange<typename AttributeContainer::const_iterator>;

    friend class AttributeContainer;

public:
    AbsVertex() = delete;
    AbsVertex( const AbsVertex& r_rhs ) = delete;
    AbsVertex& operator=( const AbsVertex& r_rhs ) = delete;

    /**
     * @return the total number of attributes of this vertex
     */
    virtual const Size numberOfAttributes() const = 0;

    /**
     * @brief containerized attribute access
     * @param index local index of the attribute
     * @return a range view containing all components of the requrested attribute
     */
    attribute_range attribute( Size index );

    /**
     * @brief containerized attribute access
     * @param index local index of the attribute
     * @return a range view containing all components of the requrested attribute
     */
    attribute_const_range attribute( Size index ) const;

    /**
     * @brief attribute access
     * 
     * @param attributeIndex
     * @param componentIndex index of the component in the specified attribute
     * 
     * @return a reference to the attribute specified by the local attribute indices
     * in the attribute container ( that gets directly loaded into the buffer )
     */
    value_type& at( Size attributeIndex,
                    Size componentIndex );

    /**
     * @brief attribute access
     * 
     * @param attributeIndex
     * @param componentIndex index of the component in the specified attribute
     * 
     * @return the value of the attribute specified by the local attribute indices
     * in the attribute container ( that gets directly loaded into the buffer )
     */
    const value_type at( Size attributeIndex,
                         Size componentIndex ) const;

    /**
     * @brief Create a new vertex that stores its attributes in the specified container
     * 
     * @param p_attributeContainer attribute container holding the attributes of the new vertex
     * @param args arguments forwarded to the vertex constructor
     * 
     * @return pointer to the new vertex
     */
    template <class VertexType, class ...Args>
    requires std::derived_from<VertexType,AbsVertex>
    friend std::shared_ptr<VertexType> makeVertex( const AttributeContainer& p_attributeContainer,
                                                   Args&&... args );
    

protected:
    AbsVertex( AttributeContainerPtr p_attributes,
               Size offset,
               const offset_container& r_attributeOffsets );

    /**
     * @brief attribute access
     * 
     * @param index attribute local index
     * 
     * @return a reference to the attribute specified by the attribute local index
     * in the attribute container ( that gets directly loaded into the buffer )
     */
    value_type& at( Size index );

    /**
     * @brief attribute access
     * 
     * @param index attribute local index
     * 
     * @return the value of the the attribute specified by the attribute local index
     * in the attribute container ( that gets directly loaded into the buffer )
     */
    const value_type at( Size index ) const;

    /**
     * @brief make space in the attribute container for the attributes
     * of this vertex.
     * @note should be called only in the constructors of derived verties
     */
    void resizeAttributeContainer();

protected:
    /**
     * Contiguous container in which the attributes of a set of vertices are stored,
     * ( including the attributes of this vertex )
     */
    AttributeContainerPtr _p_attributes;

    /**
     * Index of the first attribute of this vertex in this->_p_attributes
     */
    Size _offset;

    /**
     * Attribute layout of the vertex type.
     * Specifies the local index of the first component of each attribute
     */
    const offset_container&  _r_attributeOffsets;
};


} // namespace cie::gl

#include "ciegl/packages/buffer/impl/AbsVertex_impl.hpp"

#endif
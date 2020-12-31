#ifndef CIE_GL_BUFFER_VERTEX_2_HPP
#define CIE_GL_BUFFER_VERTEX_2_HPP

// --- Internal Includes ---
#include "ciegl/packages/buffer/inc/AbsVertex.hpp"


namespace cie::gl {


class Vertex2 : public AbsVertex
{
public:
    Vertex2( AttributeContainerPtr p_attributeContainer,
             Size offset );

    Vertex2( AttributeContainerPtr p_attributeContainer,
             Size offset,
             Vertex2::value_type x,
             Vertex2::value_type y );

    virtual const Size numberOfAttributes() const override;

    virtual Vertex2::attribute_range position();
    virtual Vertex2::attribute_const_range position() const;

    Vertex2::value_type& x();
    const Vertex2::value_type x() const;

    Vertex2::value_type& y();
    const Vertex2::value_type y() const;

protected:
    /**
     * @brief Overloaded constructor for derived Vertex classes
     */
    Vertex2( AttributeContainerPtr p_attributeContainer,
             Size offset,
             const Vertex2::offset_container& r_attributeOffsets );
};


using Vertex2Ptr = std::shared_ptr<Vertex2>;


} // namespace cie::gl

#endif
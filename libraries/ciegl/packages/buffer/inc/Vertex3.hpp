#ifndef CIE_GL_BUFFER_VERTEX_3_HPP
#define CIE_GL_BUFFER_VERTEX_3_HPP

// --- Internal Includes ---
#include "ciegl/packages/buffer/inc/Vertex2.hpp"


namespace cie::gl {


class Vertex3 : public Vertex2
{
public:
    Vertex3( AttributeContainerPtr p_attributeContainer,
             Size offset );

    Vertex3( AttributeContainerPtr p_attributeContainer,
             Size offset,
             Vertex3::value_type x,
             Vertex3::value_type y,
             Vertex3::value_type z );

    virtual const Size numberOfAttributes() const override;

    Vertex3::value_type& z();
    const Vertex3::value_type z() const;

protected:
    /**
     * @brief Overloaded constructor for derived Vertex classes
     */
    Vertex3( AttributeContainerPtr p_attributeContainer,
             Size offset,
             const Vertex3::offset_container& r_attributeOffsets );
};


using Vertex3Ptr = std::shared_ptr<Vertex3>;


} // namespace cie::gl

#endif
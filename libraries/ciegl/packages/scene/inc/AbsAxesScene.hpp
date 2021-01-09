#ifndef CIE_GL_SCENE_ABS_AXES_SCENE_HPP
#define CIE_GL_SCENE_ABS_AXES_SCENE_HPP

// --- Internal Includes ---
#include "ciegl/packages/scene/inc/Scene.hpp"
#include "ciegl/packages/buffer/inc/ColoredVertex3.hpp"
#include "ciegl/packages/traits/inc/GLTraits.hpp"

// --- STL Includes ---
#include <vector>


namespace cie::gl {


class AbsAxesScene :
    public Scene,
    public GLTraits
{
public:
    using vertex_type      = ColoredVertex3;
    using vertex_ptr       = std::shared_ptr<vertex_type>;
    using vertex_container = std::vector<vertex_ptr>;

    using scale_type       = std::vector<ColoredVertex3::value_type>;

public:
    AbsAxesScene( utils::Logger& r_logger,
                  const std::string& r_name,
                  ShaderPtr p_vertexShader,
                  ShaderPtr p_geometryShader,
                  ShaderPtr p_fragmentShader,
                  VertexBufferPtr p_vertexBuffer,
                  ElementBufferPtr p_elementBuffer );

    scale_type& scale();

    const scale_type& scale() const;

protected:
    virtual void updateAxes();

protected:
    AttributeContainerPtr _p_attributes;

    /**
     * Vertex layout:
     *  0: origin   1: x
     *  2: origin   3: y
     *  4: origin   5: z
     */
    vertex_container _vertices;

    scale_type       _scale;
    bool             _axesRequireUpdate;
};





} // namespace cie::gl


#endif
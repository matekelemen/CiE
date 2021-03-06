#ifndef CIE_SPACE_TREE_GRAPHICS_POINT_SCENE_HPP
#define CIE_SPACE_TREE_GRAPHICS_POINT_SCENE_HPP

// --- GL Includes ---
#include <ciegl/ciegl.hpp>

// --- Internal Includes ---
#include "typedefs.hpp"

// --- STL Includes ---
#include <deque>


namespace cie {



namespace detail {
template <class T>
concept CubeType
= requires ( T instance )
{
    { instance.base() };
    { instance.length() };
};

template <class T>
concept BoxType
= requires ( T instance )
{
    { instance.base() };
    { instance.lengths() };
};
} // namespace detail



class PointScene final : public gl::Scene
{
public:
    using node_container             = std::deque<csg::NodePtr>;
    using vertex_attribute_container = typename gl::VertexBuffer::data_container_type;

public:
    PointScene( const std::string& r_name,
                utils::Logger& r_logger );

    PointScene& addRoot( csg::NodePtr p_root );

    void updatePoints();

    gl::CameraPtr getCamera();

private:
    void update_impl() override;

private:
    node_container             _roots;
    vertex_attribute_container _nodeCenterAttributes;
};


} // namespace cie


#endif
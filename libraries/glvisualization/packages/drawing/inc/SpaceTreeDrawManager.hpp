#ifndef DROPMERGE_SPACE_TREE_DRAW_MANAGER_HPP
#define DROPMERGE_SPACE_TREE_DRAW_MANAGER_HPP

// --- CSG Includes ---
#include <csg/trees.hpp>

// --- Internal Includes ---
#include "glvisualization/packages/drawing/inc/DrawManager.hpp"

// --- STL Includes ---
#include <functional>



/**
 * Support cube and box cells only (for now)
*/
namespace cie::concepts {
template <class T>
concept Cube
=  PrimitiveType<T>
    && requires ( T instance )
    {
        { instance.base() }     -> std::same_as<typename T::point_type&>;
        { instance.length() }   -> std::same_as<typename T::coordinate_type&>;
    };

template <class T>
concept Box
= PrimitiveType<T>
    && requires ( T instance )
    {
        { instance.base() }     -> std::same_as<typename T::point_type&>;
        { instance.lengths() }  -> std::same_as<typename T::point_type&>;
    };
} // namespace cie::concepts



namespace cie::gl {


template <class NodeType>
class SpaceTreeDrawManager final : public gl::DrawManager
{
public:
    SpaceTreeDrawManager(   NodeType& r_root,
                            gl::GLContext& r_context  );

    void collectNodesToBuffer();
    void initialize() override;
    bool draw() override;

    void setDrawFunction( const std::function<bool()>& function );

private:
    NodeType&               _r_root;
    std::function<bool()>   _drawFunction;
};



template <concepts::Cube PrimitiveType>
typename PrimitiveType::point_type getCenter( const PrimitiveType& r_primitive );



template <concepts::Box PrimitiveType>
typename PrimitiveType::point_type getCenter( const PrimitiveType& r_primitive );



} // namespace cie::gl

#include "glvisualization/packages/drawing/impl/SpaceTreeDrawManager_impl.hpp"

#endif
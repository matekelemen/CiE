#ifndef DROPMERGE_SPACE_TREE_DRAW_MANAGER_HPP
#define DROPMERGE_SPACE_TREE_DRAW_MANAGER_HPP

// --- CSG Includes ---
#include "csg.hpp"

// --- Internal Includes ---
#include "DrawManager.hpp"

// --- STD Includes ---
#include <functional>


namespace cie {
namespace gl {


template <size_t M>
class SpaceTreeDrawManager final : public gl::DrawManager
{
public:
    SpaceTreeDrawManager(   csg::SpaceTreeNode<3,M>& root,
                            gl::GLContext& context  );

    void collectNodesToBuffer();
    void initialize() override;
    bool draw() override;

    void setDrawFunction( const std::function<bool()>& function );

private:
    csg::SpaceTreeNode<3,M>*    _root;
    std::function<bool()>       _drawFunction;
};

}
}

#include "SpaceTreeDrawManager_impl.hpp"

#endif
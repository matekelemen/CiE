#ifndef DROPMERGE_SPACE_TREE_DRAW_MANAGER_HPP
#define DROPMERGE_SPACE_TREE_DRAW_MANAGER_HPP

// --- CiE Includes ---
#include "glvisualization.hpp"
#include "csg.hpp"

// --- STD Includes ---
#include <functional>


namespace cie {
namespace csg {

const size_t M = 5;

class SpaceTreeDrawManager final : public gl::DrawManager
{
public:
    SpaceTreeDrawManager(   SpaceTreeNode<3,M>& root,
                            gl::GLContext& context  );

    void collectNodesToBuffer();
    void initialize() override;
    bool draw() override;

    void setDrawFunction( const std::function<bool()>& function );

private:
    SpaceTreeNode<3,M>*   _root;
    std::function<bool()> _drawFunction;
};


}
}

#endif
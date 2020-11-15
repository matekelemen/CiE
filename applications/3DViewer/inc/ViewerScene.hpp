#ifndef CIE_3D_VIEWER_VIEWER_SCENE_HPP
#define CIE_3D_VIEWER_VIEWER_SCENE_HPP

// --- Graphics Includes ---
#include <ciegl/ciegl.hpp>

// --- STL Includes ---
#include <list>


namespace cie {


class ViewerScene final : public gl::Scene
{
public:
    ViewerScene( utils::Logger& r_logger );

    void addModel( gl::STLPartPtr p_model );
    void removeModel( gl::STLPartPtr p_model );
    gl::CameraPtr getCamera();

private:
    void updateModels();
    void update_impl() override;

private:
    std::list<gl::STLPartPtr> _models;
    bool                      _updateModels;
};


} // namespace cie


#endif
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
    ViewerScene( const std::string& r_name,
                 utils::Logger& r_logger );

    void addModel( gl::PartPtr p_model );
    void removeModel( gl::PartPtr p_model );
    gl::CameraPtr getCamera();

private:
    void updateModels();
    void update_impl() override;

private:
    std::list<gl::PartPtr> _models;
    bool                   _updateModels;
};


} // namespace cie


#endif
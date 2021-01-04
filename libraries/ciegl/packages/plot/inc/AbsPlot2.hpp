#ifndef CIE_GL_ABS_PLOT_2_HPP
#define CIE_GL_ABS_PLOT_2_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- Internal Includes ---
#include "ciegl/packages/plot/inc/AbsPlot.hpp"
#include "ciegl/packages/buffer/inc/Vertex2.hpp"
#include "ciegl/packages/scene/inc/Scene.hpp"

#include "ciegl/packages/camera/inc/Camera.hpp"
#include "ciegl/packages/camera/inc/OrthographicProjection.hpp"
#include "ciegl/packages/control/inc/PanZoomCameraControls.hpp"

// --- STL Includes ---
#include <vector>
#include <memory>


namespace cie::gl {


template <class Tag, class ...Args>
requires Tag::value
class AbsPlot2 : public AbsPlot
{
public:
    using vertex_type         = Vertex2;
    using vertex_ptr          = Vertex2Ptr;
    using vertex_container    = std::vector<vertex_ptr>;

    using index_container     = ElementBuffer::data_container_type;
    using index_container_ptr = std::shared_ptr<index_container>;

    using camera_type         = Camera<OrthographicProjection>;
    using controls_type       = PanZoomCameraControls;
    using controls_ptr        = std::shared_ptr<controls_type>;

public:
    AbsPlot2( WindowPtr p_window );
    AbsPlot2();

    void fit() override;

private:
    void initializeScene();

protected:
    /// Internal Scene class used by AbsPlot2
    class Plot2Scene : public Scene
    {
    public:
        Plot2Scene( utils::Logger& r_logger,
                    const std::string& r_name,
                    AttributeContainerPtr p_attributes,
                    index_container_ptr p_indices );
        CameraPtr getCamera();
        
        /// Sets a flag that forces all attributes to be rebuffered on the next update
        void setUpdateFlag();

        /// Set draw mode
        void setDrawMode( GLenum drawMode );

    private:
        void update_impl() override;

    private:
        AttributeContainerPtr              _p_attributes;
        index_container_ptr                _p_indices;
        index_container                    _indices;
        bool                               _updateFlag;
        GLenum                             _drawMode;
        static const std::filesystem::path _shaderDirectory;
    };

protected:
    vertex_container            _vertices;
    index_container_ptr         _p_indices;
    CameraPtr                   _p_camera;
    std::shared_ptr<Plot2Scene> _p_scene;
    controls_ptr                _p_controls;
};


} // namespace cie::gl

#include "ciegl/packages/plot/impl/AbsPlot2_impl.hpp"

#endif
#ifndef CIE_GL_ABS_MONITOR_HPP
#define CIE_GL_ABS_MONITOR_HPP

// --- Utility Includes ---
#include "cieutils/packages/types/inc/IDObject.hpp"
#include "cieutils/packages/types/inc/types.hpp"

// --- STL Includes ---
#include <utility>
#include <memory>


namespace cie::gl {


class AbsMonitor : public utils::IDObject<Size>
{
public:
    AbsMonitor( Size id,
                Size width,
                Size height,
                bool isPrimary );

    virtual ~AbsMonitor() = 0;

    /**
     * [ width, height ]
    */
    const std::pair<Size,Size>& resolution() const;

    const bool isPrimary() const;

protected:
    std::pair<Size,Size> _resolution;
    bool _isPrimary;
};


using MonitorPtr = std::shared_ptr<AbsMonitor>;


} // namespace cie::gl


#endif
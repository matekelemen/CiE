#ifndef CIE_GL_ABS_WINDOW_HPP
#define CIE_GL_ABS_WINDOW_HPP

// --- Utility Includes ---
#include "cieutils/packages/types/inc/types.hpp"
#include "cieutils/packages/types/inc/NamedObject.hpp"
#include "cieutils/packages/types/inc/IDObject.hpp"

// --- STL Includes ---
#include <memory>
#include <utility>


namespace cie::gl {


class AbsWindow :
    public utils::IDObject<Size>,
    public utils::NamedObject
{
public:
    AbsWindow(  Size id,
                const std::string& r_name,
                Size width,
                Size height );

    virtual std::pair<double,double> getCursorPosition() = 0;

    void setSize( Size width,
                  Size height );

    const std::pair<Size,Size>& getSize() const;

protected:
    virtual void setSize_impl(  Size width,
                                Size height ) = 0;

protected:
    std::pair<Size,Size> _size;
};


using WindowPtr = std::shared_ptr<AbsWindow>;


} // namespace cie::gl


#endif
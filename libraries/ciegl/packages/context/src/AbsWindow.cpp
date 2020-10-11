// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/context/inc/AbsWindow.hpp"


namespace cie::gl {


AbsWindow::AbsWindow( Size id,
                      const std::string& r_name,
                      Size width,
                      Size height ) :
    utils::IDObject<Size>( id ),
    utils::NamedObject( r_name ),
    _size( width, height )
{
}


void AbsWindow::setSize( Size width,
                         Size height )
{
    CIE_BEGIN_EXCEPTION_TRACING

    setSize_impl( width, height );
    _size = std::make_pair( width, height );

    CIE_END_EXCEPTION_TRACING
}


const std::pair<Size,Size>& AbsWindow::getSize() const
{
    return _size;
}


} // namespace cie::gl
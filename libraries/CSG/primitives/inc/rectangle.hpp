#ifndef CSG_RECTANGLE_HPP
#define CSG_RECTANGLE_HPP

#include "csgobject.hpp"
#include "linalgtypes.hpp"

namespace cie {
namespace csg {

/*
class Rectangle : public CSGObject<2>
{
public :
    Rectangle(  const DoubleArray<2>& center, 
                const DoubleArray<2>& edgeLengths,
                double angle = 0.0);
    Rectangle();
    Rectangle( const Rectangle& copy );
    Rectangle operator=(const Rectangle& copy) const;

    double operator()( const DoubleArray<2>& point ) const override;

    const DoubleArray<2>& center() const;
    const DoubleArray<2>& edgeLengths() const;
    double angle() const;
    DoubleArray<2>& center();
    DoubleArray<2>& edgeLengths();
    double& angle();

protected:
    DoubleArray<2>  _center;
    DoubleArray<2>  _edgeLengths;
    double          _angle;
};
*/

}
}

#endif
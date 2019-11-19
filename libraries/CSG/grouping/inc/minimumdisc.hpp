#ifndef CSG_MINIMUM_DISC_HPP
#define CSG_MINIMUM_DISC_HPP

#include "linalgtypes.hpp"
#include <vector>
#include <array>
#include <memory>

namespace csg {


using PointSet2D            = std::vector<DoubleArray<2>>;
using PointSet2DPtr         = std::shared_ptr<PointSet2D>;
using ConstPointSet2DPtr    = std::shared_ptr<const PointSet2D>;


// Quick norm definition
double distance(const DoubleArray<2>& point1, const DoubleArray<2>& point2);


// Quick disc definition
struct Disc {
    Disc(const DoubleArray<2>& center, double radius);
    Disc(const DoubleArray<2>& point1, const DoubleArray<2>& point2);
    Disc(const DoubleArray<2>& point1, const DoubleArray<2>& point2, const DoubleArray<2>& point3);
    bool isInside(const DoubleArray<2>& point);

    double _radius2;
    DoubleArray<2> _center;
};


// Define class for encapsulating everything related to computing
// the minimum enclosing disc of a 2D point set
class MinimumEnclosingDisc {
public:
    MinimumEnclosingDisc(const PointSet2D& pointSet);
    void            build(double tolerance=1e-15);
    double          getRadius() const;
    DoubleArray<2>  getCenter() const;

private:
    const DoubleArray<2>&   getPoint(size_t index) const;

    Disc                _disc;
    ConstPointSet2DPtr  _pointSetPtr;
    IntArray<4>         _activeIndices;
    int                 _activeCount;
    UIntVector          _map;
};


}

#endif
#include "../inc/minimumdisc.hpp"
#include "linalgoverloads.hpp"
#include <numeric>


namespace cie {
namespace csg {


// Define the distance of two points - custom norm, no square root is needed
double distance(const DoubleArray<2>& point1, const DoubleArray<2>& point2)
{
    return linalg::norm2<DoubleArray<2>>(point1-point2);
}



Disc::Disc(const DoubleArray<2>& center, double radius) :
    _radius2(radius*radius),
    _center(center)
{
}


Disc::Disc( const DoubleArray<2>& point1, 
            const DoubleArray<2>& point2) :
    _radius2(distance(point1, point2) / 2.0),
    _center((point1 + point2) / 2.0)
{
}


Disc::Disc( const DoubleArray<2>& point1, 
            const DoubleArray<2>& point2, 
            const DoubleArray<2>& point3 )
{
    // Todo: implement disc definition by 3 points
}


bool Disc::isInside(const DoubleArray<2>& point)
{
    return distance(_center, point)<_radius2 ? true : false;
}



MinimumEnclosingDisc::MinimumEnclosingDisc( const PointSet2D& pointSet) :
    _disc({0.0,0.0},0.0),
    _pointSetPtr(std::make_shared<const PointSet2D>(pointSet)),
    _activeIndices({0,0,0,0}),
    _activeCount(0),
    _map(pointSet.size())
{
    // Initialize map and shuffle it:
    //      from this point on, accessing the points in _pointSetPtr
    //      will be rerouted through _map. No element int pointSetPtr
    //      should be accessed with operator[], the private member
    //      function getPoint will be used instead.
    std::iota(_map.begin(), _map.end(), 0);
    std::random_shuffle(_map.begin(), _map.end());

    // Fill the active set with the first 2 points
    _activeIndices[0]   = 0;
    _activeIndices[1]   = 1;
    _activeCount        = 2;

    // Define disc based on the two points
    _disc._radius2  = distance(getPoint(_activeIndices[0]), getPoint(_activeIndices[1])) / 2.0;
    _disc._center   = (getPoint(_activeIndices[0]) + getPoint(_activeIndices[1])) / 2.0;
}


void MinimumEnclosingDisc::build(double tolerance)
{
    while (true)
    {
        // Check whether all points are inside the current disc
        int outlierIndex = -1;
        for (size_t i=0; i<(*_pointSetPtr).size(); ++i )
        {
            if ( !_disc.isInside(getPoint(i)) )
            {
                outlierIndex = (int)i;
                break;
            }
        }
        if (outlierIndex==-1) 
            // Check succeeded, end the loop
            break;

        // Check failed, outlier identified
        // Todo: implement the rest of the Welzl algorithm
        //          - disc definitions missing

    }
}


double MinimumEnclosingDisc::getRadius() const 
{
    return std::sqrt(_disc._radius2);
}


DoubleArray<2> MinimumEnclosingDisc::getCenter() const
{
    return _disc._center;
}


const DoubleArray<2>& MinimumEnclosingDisc::getPoint(size_t index) const 
{
    // Reroute container indexing through the member variable: _map
    return (*_pointSetPtr)[_map[index]];
}


} // namespace csg
} // namespace cie
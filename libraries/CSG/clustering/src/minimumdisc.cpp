#include "../inc/minimumdisc.hpp"
#include "linalgoverloads.hpp"
#include <numeric>
#include <iostream>


namespace cie {
namespace csg {


// Specialize and rename linalg::norm2 for easier use
double squareNorm(const DoubleArray<2>& vector)
{
    return linalg::norm2<DoubleArray<2>>(vector);
}


// Define the distance of two points - custom norm, no square root is needed
double distance(const DoubleArray<2>& point1, const DoubleArray<2>& point2)
{
    return squareNorm(point1-point2);
}



Disc::Disc(const DoubleArray<2>& center, double radius) :
    _radius2(radius*radius),
    _center(center)
{
}


Disc::Disc( const DoubleArray<2>& point1, 
            const DoubleArray<2>& point2) :
    _radius2(distance(point1, point2) / 4.0),
    _center((point1 + point2) / 2.0)
{
}


Disc::Disc( const DoubleArray<2>& point1, 
            const DoubleArray<2>& point2, 
            const DoubleArray<2>& point3 )
{
    // Check whether the definition is unique
    double tolerance    = 1e-16;
    double determinant  =   (point1[0]-point2[0])*(point1[1]-point3[1]) 
                            - (point1[0]-point3[0])*(point1[1]-point2[1]);

    if (std::abs(determinant)<tolerance)
    {
        // Definition is not unique, either 2 or more points are the same,
        // or the 3 points are co-linear

        double d12  = distance(point1,point2);
        double d13  = distance(point1,point3);
        double d23  = distance(point2,point3);

        Disc solution({0.0,0.0},0.0);

        if (d12<tolerance)
        {
            // Point1 and point2 are equal
            solution = Disc(point1,point3);
        }
        else if (d13<tolerance)
        {
            // Point1 and point3 are equal
            solution = Disc(point1,point2);
        }
        else if (d23<tolerance)
        {
            // Point2 and point3 are equal
            solution = Disc(point1,point2);
        }
        else
        {
            // The 3 points are co-linear
            // Suppose point1 and point2 are the defining points
            if ( d12>d23 && d12>d13 )
            {
                solution = Disc(point1,point2);
            }
            else if ( d13>d12 && d13>d23 )
            {
                // Nope, it was point1 and point3
                solution = Disc(point1,point3);
            }
            else
            {
                // Nope, it was point2 and point3
                solution = Disc(point2,point3);
            }
            
        }

        _center     = solution._center;
        _radius2    = solution._radius2;
        return;
    }

    // The disc definition is unique

    // Circle center defined by 3 points on its arc:
    //      --            -- ^(-1)    --                         --
    //   2* | x1-x2  y1-y2 |        * | (x1^2+y1^2) - (x2^2+y2^2) |
    //      | x1-x3  y1-y3 |          | (x1^2+y1^2) - (x3^2+y3^2) |
    //      --            --          --                         --

    // Calculate center
    _center = 
        linalg::Matrix(     {   point1[1]-point3[1], point2[1]-point1[1],
                                point3[0]-point1[0], point1[0]-point2[0]},
                            2 )
        *
        DoubleArray<2>( {   squareNorm(point1) - squareNorm(point2),
                            squareNorm(point1) - squareNorm(point3) })
        /
        determinant
        /
        2.0;

    // Calculate radius based on the center
    _radius2 = distance(_center, point1);

    /*
    // DEBUG: check if the other two points are as far from the center as the first one
    double tolerance = 1e-16;
    if ( std::abs(distance(_center,point2)-_radius2)>tolerance || std::abs(distance(_center,point3)-_radius2)>tolerance )
        throw std::runtime_error("Disc center calculation failed!");
    */
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
#ifndef OPTIMIZATION_BASIC_TYPES_HPP
#define OPTIMIZATION_BASIC_TYPES_HPP

#include <vector>
#include <array>
#include <cmath>

namespace opt {

typedef unsigned int            uint;

typedef std::array<double, 2>   Point2;
typedef std::array<double, 3>   Point3;

typedef std::vector<int>    IntVector;
typedef std::vector<double> DoubleVector;


double norm2(const DoubleVector& vector);
void normalize(DoubleVector& vector);

}

#endif
#ifndef OPT_RANDOM_UTILITIES_HPP
#define OPT_RANDOM_UTILITIES_HPP


#include <ctime>
#include <random>

#include "linalgtypes.hpp"

namespace opt {

// ---------------------------------------------------------------
// INTEGER GENERATORS
// ---------------------------------------------------------------
int randomUniformInteger(int minVal, int maxVal);

// ---------------------------------------------------------------
// FLOATING POINT GENERATORS
// ---------------------------------------------------------------
double randomUniformDouble(double minVal, double maxVal);
std::unique_ptr<DoubleVector> randomUniformDoubleVector(double minVal, double maxVal, uint size);

double randomNormalDouble(double mean, double deviation);
std::unique_ptr<DoubleVector> randomNormalDoubleVector(double mean, double deviation, uint size);

}


#endif
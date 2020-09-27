#ifndef OPT_RANDOM_UTILITIES_HPP
#define OPT_RANDOM_UTILITIES_HPP

// --- Linalg Includes ---
#include "linalg/packages/types/inc/vectortypes.hpp"

// --- Utility Includes ---
#include "cieutils/packages/types/inc/types.hpp"

// --- STL Includes ---
#include <ctime>
#include <random>
#include <memory>


namespace cie::opt {

// ---------------------------------------------------------------
// INTEGER GENERATORS
// ---------------------------------------------------------------
int randomUniformInteger(int minVal, int maxVal);

// ---------------------------------------------------------------
// FLOATING POINT GENERATORS
// ---------------------------------------------------------------
double randomUniformDouble(double minVal, double maxVal);
std::unique_ptr<DoubleVector> randomUniformDoubleVector(double minVal, double maxVal, Size size);

double randomNormalDouble(double mean, double deviation);
std::unique_ptr<DoubleVector> randomNormalDoubleVector(double mean, double deviation, Size size);

} // namespace cie::opt

#endif
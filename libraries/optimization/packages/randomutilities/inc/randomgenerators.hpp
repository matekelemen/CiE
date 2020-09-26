#ifndef OPT_RANDOM_UTILITIES_HPP
#define OPT_RANDOM_UTILITIES_HPP

// --- Linalg Includes ---
#include "linalg/packages/types/inc/vectortypes.hpp"

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
std::unique_ptr<DoubleVector> randomUniformDoubleVector(double minVal, double maxVal, uint size);

double randomNormalDouble(double mean, double deviation);
std::unique_ptr<DoubleVector> randomNormalDoubleVector(double mean, double deviation, uint size);

} // namespace cie::opt

#endif
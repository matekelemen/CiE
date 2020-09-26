// --- Internal Includes ---
#include "optimization/packages/randomutilities/inc/randomgenerators.hpp"

// --- STL Includes ---
#include <memory>

namespace cie::opt {

// ---------------------------------------------------------------
// GENERATOR REQUIREMENTS
// ---------------------------------------------------------------

static uint64_t generatorState[2] = {
    uint64_t(std::time(NULL)) + 987654321,
    123456789
};
std::random_device randomDevice;
std::mt19937 randomGenerator(randomDevice());


// ---------------------------------------------------------------
// INTEGER GENERATORS
// ---------------------------------------------------------------

inline int randomUniformInteger(int minVal, int maxVal){
    std::uniform_int_distribution<int> distribution(minVal,maxVal);
    return distribution(randomGenerator);
}


// ---------------------------------------------------------------
// FLOATING POINT GENERATORS
// ---------------------------------------------------------------

double randomUniformDouble(double minVal, double maxVal){
    std::uniform_real_distribution<double> distribution(minVal,maxVal);
    return distribution(randomGenerator);
}


std::unique_ptr<DoubleVector> randomUniformDoubleVector(double minVal, double maxVal, uint size){
    std::uniform_real_distribution<double> distribution(minVal,maxVal);
    std::unique_ptr<DoubleVector>  vector(std::make_unique<DoubleVector>(size));
    for(auto it=vector->begin(); it!=vector->end(); ++it){
        *it = distribution(randomGenerator);
    }
    return vector;
}


double randomNormalDouble(double mean, double deviation){
    std::normal_distribution<double> distribution(mean,deviation);
    return distribution(randomGenerator);
}


std::unique_ptr<DoubleVector> randomNormalDoubleVector(double mean, double deviation, uint size){
    std::normal_distribution<double> distribution(mean,deviation);
    std::unique_ptr<DoubleVector>  vector(std::make_unique<DoubleVector>(size));
    for(auto it=vector->begin(); it!=vector->end(); ++it){
        *it = distribution(randomGenerator);
    }
    return vector;
}


} // namespace cie::opt
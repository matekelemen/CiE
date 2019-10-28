#ifndef OPTIMIZATION_CHROMOSOME_HPP
#define OPTIMIZATION_CHROMOSOME_HPP

#include "basictypes.hpp"

namespace opt {

// Solution representation interface
class Chromosome {
public:
    Chromosome();

    void setObjective(double newObjective);
    double getObjective() const;
    
protected:
    Chromosome(double objective);

    double _objective;
};

}

#endif
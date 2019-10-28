#include "chromosome.hpp"

namespace opt {

Chromosome::Chromosome() :
    _objective(0.0)
{
}

void Chromosome::setObjective(double newObjective){
    _objective = newObjective;
}

double Chromosome::getObjective() const {
    return _objective;
}

Chromosome::Chromosome(double objective) :
    _objective(objective)
{
}

}
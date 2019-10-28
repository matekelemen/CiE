#include "basictypes.hpp"

namespace opt {


double norm2(const DoubleVector& vector){
    double result = 0.0;
    for (auto it=vector.begin(); it!=vector.end(); ++it) result += (*it)*(*it);
    return result;
}


void normalize(DoubleVector& vector){
    double norm = sqrt(norm2(vector));
    if (abs(norm)>1e-15)
        for (auto it=vector.begin(); it!=vector.end(); ++it) (*it)/=norm;
    else
        throw std::runtime_error("Cannot normalize vector with zero length!");
}


}
#ifndef OPTIMIZATION_ELEMENT_HPP
#define OPTIMIZATION_ELEMENT_HPP

#include "linalgtypes.hpp"

namespace cie {
namespace opt {

// Solution representation interface
template <typename DataType, typename ObjectiveType>
class Element {
public:
    Element(const DataType& data);
    Element();

    Element(const Element<DataType, ObjectiveType>& toCopy);
    Element<DataType,ObjectiveType> operator=(const Element<DataType,ObjectiveType>& toCopy) const;

    void setData(const DataType& data);
    DataType& getData();
    const DataType& getData() const;

    void setObjective(const ObjectiveType& objective);
    const ObjectiveType& getObjective() const;
    
protected:
    Element(const DataType& data, double, const ObjectiveType& objective);

    DataType _data;
    ObjectiveType _objective;
};



// Common element specializations
using RRElement         = Element<double, double>;

template <size_t N>
using RNRElement        = Element<DoubleArray<N>, double>;

template <size_t N, size_t M>
using RNRMElement       = Element<DoubleArray<N>, const DoubleArray<M>>;

using RNRVectorElement  = Element<DoubleVector, double>;

using RNRMVectorElement = Element<DoubleVector, const DoubleVector>;


} // namespace opt
}

#include "element_impl.hpp"

#endif
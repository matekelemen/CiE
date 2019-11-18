#ifndef OPTIMIZATION_ELEMENT_IMPL_HPP
#define OPTIMIZATION_ELEMENT_IMPL_HPP

namespace opt {

template <typename DataType, typename ObjectiveType>
Element<DataType,ObjectiveType>::Element(const DataType& data, double, const ObjectiveType& objective) :
    _data(),
    _objective(objective)
{
}


template <typename DataType, typename ObjectiveType>
Element<DataType,ObjectiveType>::Element(const DataType& data) :
    _data(data),
    _objective()
{
}


template <typename DataType, typename ObjectiveType>
Element<DataType,ObjectiveType>::Element() :
    _data(),
    _objective()
{
}


template <typename DataType, typename ObjectiveType>
void Element<DataType,ObjectiveType>::setData(const DataType& data)
{
    _data = data;
}


template <typename DataType, typename ObjectiveType>
const DataType& Element<DataType,ObjectiveType>::getData() const 
{
    return _data;
}


template <typename DataType, typename ObjectiveType>
void Element<DataType,ObjectiveType>::setObjective(const ObjectiveType& objective){
    _objective = objective;
}


template <typename DataType, typename ObjectiveType>
const ObjectiveType& Element<DataType,ObjectiveType>::getObjective() const {
    return _objective;
}

    
} // namespace opt

#endif
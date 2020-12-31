#ifndef CIE_FEM_DISCRETIZATION_BASIS_WRAPPERS_HPP
#define CIE_FEM_DISCRETIZATION_BASIS_WRAPPERS_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"
#include "cieutils/packages/stl_extension/inc/state_iterator.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"
#include "cieutils/packages/exceptions/inc/exception.hpp"

// --- STL Includes ---
#include <algorithm>
#include <array>


namespace cie::fem::detail {

// Helper class for accessing basis values
template <  concepts::STLContainer ContainerType,
            Size Dimension >
class TensorProductBasis 
    : public cie::utils::StateIterator<typename std::array<ContainerType,Dimension>::const_iterator>
{
public:
    typedef std::array<ContainerType,Dimension>     container_array;
    typedef typename container_array::value_type    container_type;
    typedef typename container_type::value_type     value_type;

public:
    TensorProductBasis( const container_array& array ) :
        cie::utils::StateIterator<typename std::array<ContainerType,Dimension>::const_iterator>( array.begin(), array.end() )
    {}

    // Product of all current state components
    value_type product() const
    {
        value_type output = 1.0;
        for (auto it : this->_state)
            output *= (*it);
        return output;
    }
    
};


// Helper class for accessing derivatives of a tensor product basis
template <  concepts::STLContainer ContainerType,
            Size Dimension >
class TensorProductDerivatives
{
public:
    static const Size                                                               dimension = Dimension;
    typedef std::array<ContainerType,Dimension>                                     container_array;
    typedef typename container_array::value_type                                    container_type;
    typedef typename container_type::value_type                                     value_type;
    typedef std::array<value_type,Dimension>                                        point_type;
    typedef cie::utils::StateIterator<typename container_array::const_iterator>     state_iterator;

public:
    TensorProductDerivatives(   const container_array& basisValues,
                                const container_array& derivativeValues ) :
        _basisState( basisValues.begin(), basisValues.end() ),
        _derivativeState( derivativeValues.begin(), derivativeValues.end() )
    {
        #ifdef CIE_ENABLE_OUT_OF_RANGE_CIE_TEST_CHECKS
            CIE_ASSERT(
                basisValues.size() == derivativeValues.size(),
                "dimension of basis functions and their derivatives must be identical"
            )
            for (Size i=0; i<basisValues.size(); ++i)
            {
                CIE_ASSERT(
                    basisValues[i].size() == derivativeValues[i].size(),
                    "number of basis function values and their derivatives must match"
                )
            }
        #endif
        this->reset();
    }

    point_type product() const
    {
        point_type derivative;
        std::fill(  derivative.begin(),
                    derivative.end(),
                    1.0 );

        // Compute product
        for (Size componentIndex=0; componentIndex<dimension; ++componentIndex)
            for (Size dimensionIndex=0; dimensionIndex<dimension; ++dimensionIndex)
            {
                if (componentIndex == dimensionIndex)
                    derivative[componentIndex] *= *(*_derivativeState)[dimensionIndex];
                else
                    derivative[componentIndex] *= *(*_basisState)[dimensionIndex];
            }

        return derivative;
    }

    void reset()
    {
        _basisState.reset();
        _derivativeState.reset();
    }

    TensorProductDerivatives& operator++()
    {
        ++_basisState;
        ++_derivativeState;
        return *this;
    } // operator++


private:
    state_iterator  _basisState;
    state_iterator  _derivativeState;
}; // class TensorProductDerivatives



template <class ContainerType, Size Dimension>
inline TensorProductBasis<ContainerType,Dimension>
makeTensorProductBasis( const std::array<ContainerType,Dimension>& containerArray )
{
    return TensorProductBasis<ContainerType,Dimension>( containerArray );
}


template <class ContainerType, Size Dimension>
inline TensorProductDerivatives<ContainerType,Dimension>
makeTensorProductDerivatives(   const std::array<ContainerType,Dimension>& basisValues,
                                const std::array<ContainerType,Dimension>& derivativeValues )
{
    return TensorProductDerivatives<ContainerType,Dimension>( basisValues, derivativeValues );
}



} // namespace cie::fem::detail


#endif
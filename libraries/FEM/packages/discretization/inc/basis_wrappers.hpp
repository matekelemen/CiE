#ifndef CIE_FEM_DISCRETIZATION_BASIS_WRAPPERS_HPP
#define CIE_FEM_DISCRETIZATION_BASIS_WRAPPERS_HPP

// --- Utility Includes ---
#include <cieutils/concepts.hpp>

// --- STL Includes ---
#include <algorithm>


namespace cie::fem {


// Helper class for accessing basis values
namespace detail {
template <  concepts::STLContainer ContainerType,
            Size Dimension >
class TensorProductBasis
{
private:
    typedef std::array<ContainerType,Dimension>     container_array;
    typedef typename container_array::value_type    container_type;
    typedef typename container_type::value_type     value_type;
    typedef typename container_type::const_iterator iterator_type;

public:
    TensorProductBasis( const container_array& array ) :
        _array(&array)
    {
        this->reset();
    }

    value_type operator*()
    {
        // Compute product
        value_type value(1.0);
        for (auto valueIt : _state)
            value *= *valueIt;

        return value;
    }

    void reset()
    {
        std::transform( _array->begin(),
                        _array->end(),
                        _state.begin(),
                        [](const auto& container)
                        {
                            return container.begin();
                        });
    }

    TensorProductBasis& operator++()
    {
        auto stateIt    = _state.end()-1;
        auto dimIt      = _array->end()-1;

        // Increment state
        while ( stateIt != _state.begin()-1 )
        {
            if ( (*stateIt)+1 != dimIt->end() )
            {
                (*stateIt)++;
                break;
            }
            else
            {
                *stateIt = dimIt->begin();
                --dimIt;
                --stateIt;
            }
        } // while (true)

        return *this;
    } // void operator++


private:
    const container_array* const        _array;
    std::array<iterator_type,Dimension> _state;
};


// Helper class for accessing derivatives of a tensor product basis
template <  concepts::STLContainer ContainerType,
            Size Dimension >
class TensorProductDerivatives
{
private:
    static const Size                               dimension = Dimension;
    typedef std::array<ContainerType,Dimension>     container_array;
    typedef typename container_array::value_type    container_type;
    typedef typename container_type::value_type     value_type;
    typedef typename container_type::const_iterator iterator_type;
    typedef std::array<value_type,Dimension>        point_type;

public:
    TensorProductDerivatives(   const container_array& basisValues,
                                const container_array& derivativeValues ) :
        _basis(&basisValues),
        _derivatives(&derivativeValues)
    {
        this->reset();
    }

    point_type operator*()
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
                    derivative[componentIndex] *= *_derivativeState[dimensionIndex];
                else
                    derivative[componentIndex] *= *_basisState[dimensionIndex];
            }

        return derivative;
    }

    void reset()
    {
        std::transform( _basis->begin(),
                        _basis->end(),
                        _basisState.begin(),
                        [](const auto& container)
                        {
                            return container.begin();
                        });
        std::transform( _derivatives->begin(),
                        _derivatives->end(),
                        _derivativeState.begin(),
                        [](const auto& container)
                        {
                            return container.begin();
                        });
    }

    TensorProductDerivatives& operator++()
    {
        auto basisStateIt       = _basisState.end()-1;
        auto basisIt            = _basis->end()-1;
        auto derivativeStateIt  = _derivativeState.end()-1;
        auto derivativeIt       = _derivatives->end()-1;

        // Increment state
        while ( basisStateIt != _basisState.begin()-1 )
        {
            if ( (*basisStateIt)+1 != basisIt->end() )
            {
                (*basisStateIt)++;
                (*derivativeStateIt)++;
                break;
            }
            else
            {
                *basisStateIt       = basisIt->begin();
                *derivativeStateIt  = derivativeIt->begin();
                --basisIt;
                --basisStateIt;
                --derivativeIt;
                --derivativeStateIt;
            }
        } // while (true)

        return *this;
    } // void operator++


private:
    const container_array* const        _basis;
    const container_array* const        _derivatives;
    std::array<iterator_type,Dimension> _basisState;
    std::array<iterator_type,Dimension> _derivativeState;
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



} // namespace detail


} // namespace cie::fem


#endif
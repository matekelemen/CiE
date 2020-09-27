#ifndef CIE_FEM_PYFEM_CONVENIENCE_FUNCTIONS_HPP
#define CIE_FEM_PYFEM_CONVENIENCE_FUNCTIONS_HPP

// --- STL Includes ---
#include <deque>


namespace cie::fem {


template <class ElementType>
struct ElementContainer : public std::deque<ElementType> {};


/**
 * Call integrateStiffness on a list of elements.
 * Integration is done in parallel but the updates are
 * sent to a buffer container first. After all elements
 * are done with integration, updates are applied on a 
 * single thread (writing from different threads, even
 * from only one at a time, breaks python -.-).
*/
template <class ElementType>
void
integrateStiffness( ElementContainer<ElementType>& elementContainer,
                    typename ElementType::matrix_update_function updateFunction )
{
    // Define matrix update container
    std::deque<std::pair
    <
        std::pair<Size,Size>,
        typename ElementType::NT
    >> updates;

    // Define function that pushes updates to the container
    auto recordUpdate = [&updates]( Size i, Size j, typename ElementType::NT value )
    {
        #pragma omp critical
        { updates.emplace_back( std::make_pair(std::make_pair(i,j), value) ); }
    };

    // Integrate
    #ifndef MSVC
    #pragma omp parallel for shared(elementContainer,updateFunction)
    #endif
    for (auto& element : elementContainer)
    {
        element.integrateStiffness(recordUpdate);
    }

    // Apply updates to external object
    for (const auto& update : updates)
        updateFunction( update.first.first, update.first.second, update.second );
} // void integrateStiffness


/**
 * Call integrateLoad on a list of elements.
 * Integration is done in parallel but the updates are
 * sent to a buffer container first. After all elements
 * are done with integration, updates are applied on a 
 * single thread (writing from different threads, even
 * from only one at a time, breaks python -.-).
*/
template <class ElementType>
void
integrateLoad(  ElementContainer<ElementType>& elementContainer,
                typename ElementType::vector_update_function updateFunction )
{
    // Define vector update container
    std::deque<std::pair
    <
        Size,
        typename ElementType::NT
    >> updates;

    // Define function that pushes updates to the container
    auto recordUpdate = [&updates]( Size i, typename ElementType::NT value )
    {
        #pragma omp critical
        { updates.emplace_back( std::make_pair(i, value) ); }
    };

    // Integrate
    #ifndef MSVC
    #pragma omp parallel for shared(elementContainer,updateFunction)
    #endif
    for (auto& element : elementContainer)
    {
        element.integrateLoad(recordUpdate);
    }

    // Apply updates to external object
    for (const auto& update : updates)
        updateFunction( update.first, update.second );
} // void integrateStiffness


} // namespace cie::fem


#endif
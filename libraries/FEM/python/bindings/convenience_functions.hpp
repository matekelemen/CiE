#ifndef CIE_FEM_PYFEM_CONVENIENCE_FUNCTIONS_HPP
#define CIE_FEM_PYFEM_CONVENIENCE_FUNCTIONS_HPP

// --- STL Includes ---
#include <deque>


namespace cie::fem {


template <class ElementType>
struct ElementList : public std::deque<ElementType> {};


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
integrateStiffness( ElementList<ElementType>& elementList,
                    typename ElementType::matrix_update_function updateFunction )
{
    std::deque<std::pair
    <
        std::pair<Size,Size>,
        typename ElementType::NT
    >> updates;
    auto recordUpdate = [&updates]( Size i, Size j, typename ElementType::NT value )
    {
        #pragma omp critical
        { updates.emplace_back( std::make_pair(std::make_pair(i,j), value) ); }
    };

    #pragma omp parallel for shared(elementList,updateFunction)
    for (auto& element : elementList)
    {
            element.integrateStiffness(recordUpdate);
    }

    for (const auto& update : updates)
        updateFunction( update.first.first, update.first.second, update.second );
} // void integrateStiffness


} // namespace cie::fem


#endif
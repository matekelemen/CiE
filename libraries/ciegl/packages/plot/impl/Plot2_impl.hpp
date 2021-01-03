#ifndef CIE_GL_PLOT_PLOT_2_IMPL_HPP
#define CIE_GL_PLOT_PLOT_2_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"

// --- STL Includes ---
#include <limits>


namespace cie::gl {


template <class ...Args>
Plot2<Args...>::Plot2( Args&&... args ) :
    AbsPlot2()
{
    CIE_THROW( NotImplementedException, "No specialization was found for Plot2 and the specified arguments" )
}



template < concepts::NumericContainer XContainer,
           concepts::NumericContainer YContainer >
Plot2<XContainer,YContainer>::Plot2( const XContainer& r_xContainer,
                                     const YContainer& r_yContainer ) :
    AbsPlot2()
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Check data sizes
    CIE_CHECK(
        r_xContainer.size() == r_yContainer.size(),
        "Container size mismatch! " 
            + std::to_string(r_xContainer.size()) + " != "
            + std::to_string(r_yContainer.size())
    )

    CIE_CHECK(
        r_xContainer.size() > 0,
        "Cannot plot empty data"
    )

    // Initialize plot
    this->_vertices.reserve( r_xContainer.size() );

    {
        auto p_vertex = makeVertex<Plot2::vertex_type>(
            this->_p_attributes,
            r_xContainer[0],
            r_yContainer[0]
        );
        this->_p_attributes->reserve( r_xContainer.size() * p_vertex->numberOfAttributes() );
        this->_vertices.push_back( p_vertex );
    }

    // Load data
    for ( Size vertexIndex=1; vertexIndex<r_xContainer.size(); ++vertexIndex )
        this->_vertices.push_back( makeVertex<Plot2::vertex_type>(
            this->_p_attributes,
            r_xContainer[vertexIndex],
            r_yContainer[vertexIndex]
        ) );

    // Adjust camera
    this->fit();

    this->_p_window->beginLoop();

    CIE_END_EXCEPTION_TRACING
}


template < concepts::NumericContainer XContainer,
           concepts::NumericContainer YContainer >
void plot2( const XContainer& r_XContainer,
            const YContainer& r_YContainer )
{
    Plot2<XContainer,YContainer>( r_XContainer, r_YContainer );
}


} // namespace cie::gl


#endif
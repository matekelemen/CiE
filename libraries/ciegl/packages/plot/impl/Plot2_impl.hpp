#ifndef CIE_GL_PLOT_PLOT_2_IMPL_HPP
#define CIE_GL_PLOT_PLOT_2_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/checks.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"

// --- STL Includes ---
#include <limits>


namespace cie::gl {


template < concepts::NumericContainer XContainer,
           concepts::NumericContainer YContainer >
void plot( const XContainer& r_xContainer,
               const YContainer& r_yContainer )
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
    auto p_plot = Plot2Ptr( new Plot2 );
    p_plot->_vertices.reserve( r_xContainer.size() );

    {
        auto p_vertex = makeVertex<Plot2::vertex_type>(
            p_plot->_p_attributes,
            r_xContainer[0],
            r_yContainer[0]
        );
        p_plot->_p_attributes->reserve( r_xContainer.size() * p_vertex->numberOfAttributes() );
        p_plot->_vertices.push_back( p_vertex );
    }

    // Load data
    for ( Size vertexIndex=1; vertexIndex<r_xContainer.size(); ++vertexIndex )
        p_plot->_vertices.push_back( makeVertex<Plot2::vertex_type>(
            p_plot->_p_attributes,
            r_xContainer[vertexIndex],
            r_yContainer[vertexIndex]
        ) );

    // Adjust camera
    p_plot->fit();

    p_plot->_p_window->beginLoop();

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::gl


#endif
#ifndef LINALG_UTILITIES_IMPL_HPP
#define LINALG_UTILITIES_IMPL_HPP

//#include "utilities.hpp" // prevent IDE from complaining

namespace linalg {

namespace linalghelper {

template<typename VectorType>
void writeRow( const VectorType& vector, size_t size, std::ostream& out, size_t digits )
{
    auto precision = out.precision( );

    out << std::setprecision( digits - 4 );

    for( size_t i = 0; i < size; ++i )
    {
        out << std::setw( digits ) << vector( i );
    }

    out << std::endl << std::setprecision( precision );
}

}

}

#endif
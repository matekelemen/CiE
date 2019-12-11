#ifndef LINALG_HELPER_IMPL_HPP
#define LINALG_HELPER_IMPL_HPP

namespace cie {
namespace linalg {
namespace linalghelper {


template<typename DoubleVectorType>
void writeRow( const DoubleVectorType& vector, size_t size, std::ostream& out, size_t digits )
{
    auto precision = out.precision( );

    out << std::setprecision( digits - 4 );

    for( size_t i = 0; i < size; ++i )
    {
        //out << std::setw( digits ) << vector( i );
        out << vector( i );
        if (i<size-1) out << ',';
    }

    out << std::endl << std::setprecision( precision );
}


} // namespace linalghelper
} // namespace linalg
}

#endif
#ifndef LINALG_HELPER_IMPL_HPP
#define LINALG_HELPER_IMPL_HPP

namespace linalg {
namespace linalghelper {


template<typename VectorType>
void writeRow( const VectorType& vector, size_t size, std::ostream& out, size_t digits )
{
    auto precision = out.precision( );

    out << std::setprecision( digits - 4 );

    for( size_t i = 0; i < size; ++i )
    {
        //out << std::setw( digits ) << vector( i );
        out << vector( i ) << ",";
    }

    out << std::endl << std::setprecision( precision );
}


} // namespace linalghelper
} // namespace linalg

#endif
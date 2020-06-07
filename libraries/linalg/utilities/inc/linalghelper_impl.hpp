#ifndef CIE_LINALG_HELPER_IMPL_HPP
#define CIE_LINALG_HELPER_IMPL_HPP

namespace cie {
namespace linalg {
namespace linalghelper {


template<class RowFunction>
void writeRow( const RowFunction& vector, Size size, std::ostream& out, Size digits )
{
    auto precision = out.precision( );

    out << std::setprecision( digits - 4 );

    for( Size i = 0; i < size; ++i )
    {
        out << std::setw( digits ) << vector( i );
        if (i<size-1)
            out << ",";
    }

    out << std::endl << std::setprecision( precision );
}


template<concepts::NumericContainer ContainerType>
void write( const ContainerType& vector, std::ostream& out )
{
    linalghelper::writeRow( [&]( size_t i ){ return vector[i]; }, vector.size( ), out, 12 );
}


template<concepts::NumericType ValueType>
void write( const Matrix<ValueType>& matrix, std::ostream& out )
{
    Size size1 = matrix.size1( );
    Size size2 = matrix.size2( );

    for( Size i = 0; i < size1; ++i )
        linalghelper::writeRow( [&]( Size j ){ return matrix( i, j ); }, size2, out, 12 );
}


} // namespace linalghelper
} // namespace linalg
}

#endif
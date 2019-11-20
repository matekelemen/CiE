#include "../inc/linalghelper.hpp"

namespace cie {
namespace linalg {
namespace linalghelper {


void write( const DoubleVector& vector, std::ostream& out )
{
    linalghelper::writeRow( [&]( size_t i ){ return vector[i]; }, vector.size( ), out, 12 );
}


void write( const Matrix& matrix, std::ostream& out )
{
    size_t size1 = matrix.size1( );
    size_t size2 = matrix.size2( );

    for( size_t i = 0; i < size1; ++i )
    {
        linalghelper::writeRow( [&]( size_t j ){ return matrix( i, j ); }, size2, out, 12 );
    }
}


} // namespace linalghelper
} // namespace linalg
}
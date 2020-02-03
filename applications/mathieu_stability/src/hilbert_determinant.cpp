#include "../inc/hilbert_determinant.hpp"

namespace cie {
namespace mathieu {


Complex hilbertDeterminant( double delta,
                            double epsilon,
                            double gamma,
                            double omega,
                            double omegaCritical,
                            size_t N)
{
    double temp = omegaCritical - ((double)N)*omega;
    double subDiag  = epsilon * epsilon / 4.0;
    
    Complex determinant0( delta-temp*temp, gamma*temp );

    temp            = omegaCritical - (double)(N-1)*omega;
    Complex determinant1 = Complex(delta - temp * temp, gamma*temp ) * determinant0;
    determinant1    -= subDiag;

    Complex swap;
    
    // Recursively compute the determinant of a tridiagonal matrix
    for (int k = -1*(int)(N-2); k <= (int)N; ++k)
    {
        temp    = k*omega + omegaCritical;

        swap    =   Complex(delta-temp*temp, gamma*temp) * determinant1
                    - subDiag * determinant0;

        determinant0 = determinant1;
        determinant1 = swap;
    }

    return determinant1;
}


}
}
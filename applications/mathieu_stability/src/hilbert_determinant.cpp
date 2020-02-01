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
    // Initialize the first two subdeterminants
    double temp     = omegaCritical - N*omega;
    double subDiag  = epsilon * epsilon / 4.0;

    Complex determinant0(   delta - temp * temp,
                            gamma * temp );

    temp            += omega;
    Complex determinant1 = Complex(delta - temp * temp, gamma * temp ) * determinant0;
    determinant1    -= subDiag;
        
    // Recursively loop through the band
    Complex swap;
    
    for (size_t i = 2; i < 2 * N + 1; ++i)
    {
        temp    += omega;
        swap    =   Complex(delta-temp*temp, gamma*temp) * determinant1
                    - subDiag * determinant0;

        determinant0 = determinant1;
        determinant1 = swap;
    }

    return determinant1;
}


}
}
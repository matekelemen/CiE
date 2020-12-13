// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "linalg/packages/decomposition/inc/QRFactorization.hpp"
#include "linalg/packages/utilities/inc/linalghelper.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <iostream>
#include <fstream>


namespace cie::linalg {

CIE_TEST_CASE( "QR Factorization", "[decomposition]" )
{
    Matrix matrix(  {   1.0,4.0,9.0,
                        1.0,1.0,2.0,
                        3.0,5.0,8.0},
                    3   );
    
    auto qr = QRFactorization(matrix);
    
    std::ofstream QFile( TEST_OUTPUT_PATH / "QR_factorization_Q.csv");
    linalghelper::write(*(qr.first),QFile);
    QFile.close();

    std::ofstream RFile(TEST_OUTPUT_PATH / "QR_factorization_R.csv");
    linalghelper::write(*(qr.second),RFile);
    RFile.close();
}

}
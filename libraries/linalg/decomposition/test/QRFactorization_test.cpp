#include "catch.hpp"
#include "../inc/QRFactorization.hpp"
#include "../../utilities/inc/linalghelper.hpp"
#include <iostream>
#include <fstream>

namespace cie {
namespace linalg {

TEST_CASE("QR Factorization")
{
    Matrix matrix(  {   1.0,4.0,9.0,
                        1.0,1.0,2.0,
                        3.0,5.0,8.0},
                    3   );
    
    auto qr = QRFactorization(matrix);
    
    std::ofstream QFile("Q.csv");
    linalghelper::write(*(qr.first),QFile);
    QFile.close();

    std::ofstream RFile("R.csv");
    linalghelper::write(*(qr.second),RFile);
    RFile.close();
}

}
}
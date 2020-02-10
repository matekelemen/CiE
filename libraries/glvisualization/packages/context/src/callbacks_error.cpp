#include "../inc/callbacks_error.hpp"
#include <iostream>
#include <string>

namespace cie {
namespace gl {


void callback_errorPrint(int error, const char* description)
{
    std::cout << description << " (error code " + std::to_string( error ) + ")\n";
}


}
}
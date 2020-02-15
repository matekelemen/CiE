#include "../inc/GLLogger.hpp"
#include <iostream>
#include <chrono>
#include <ctime>


namespace cie {
namespace gl {


GLLogger::GLLogger( const std::string& fileName ) :
    _file(fileName, std::ofstream::out)
{
    if (_file.is_open())
    {
        auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        _file << std::ctime(&t);
    }
    else 
    {
        throw std::runtime_error("Failed to open logger file " + fileName );
    }
}


GLLogger::~GLLogger()
{
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    _file << std::ctime(&t);
    _file.close();
}


void GLLogger::report( const std::string& message )
{
    _file << message << "\n";
}


void GLLogger::print( const std::string& message ) const
{
    std::cout << message << "\n";
}


void GLLogger::warn( const std::string& message )
{
    _file << "WARNING: " << message << "\n";
    std::cout << "WARNING: " << message << "\n";
}


void GLLogger::error( const std::string& message )
{
    _file << "ERROR: " << message << "\n";
    _file.close();
    throw std::runtime_error( message );
}





}
}
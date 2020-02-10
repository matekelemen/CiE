#include "../inc/GLLogger.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>


namespace cie {
namespace gl {


GLLogger::GLLogger( const std::string& fileName ) :
    _file(fileName, std::ofstream::out)
{
    if (_file.is_open())
    {
        auto t      = std::time(nullptr);
        auto time   = *std::localtime(&t);
        _file << std::put_time(&time, "%d-%m-%Y %H-%M-%S");
        _file << "\n";
    }
    else 
    {
        throw std::runtime_error("Failed to open logger file " + fileName );
    }
}


GLLogger::~GLLogger()
{
    auto t      = std::time(nullptr);
    auto time   = *std::localtime(&t);
    _file << std::put_time(&time, "%d-%m-%Y %H-%M-%S");
    _file.close();
}


void GLLogger::report( const std::string& message )
{
    _file << message << "\n";
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
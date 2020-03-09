#ifndef GLVISUALIZATION_GL_LOGGER
#define GLVISUALIZATION_GL_LOGGER

#include <fstream>
#include <string>

namespace cie {
namespace gl {


class GLLogger
{
public:
    GLLogger( const std::string& fileName );
    ~GLLogger();

    void report( const std::string& message );
    void print( const std::string& message ) const;
    void warn( const std::string& message );
    void error( const std::string& message );

    void close();

private:
    std::ofstream _file;
};


}
}


#endif
#ifndef GLVISUALIZATION_GL_LOGGER
#define GLVISUALIZATION_GL_LOGGER

#include <fstream>

namespace cie {
namespace gl {


class GLLogger
{
public:
    GLLogger( const std::string& fileName );
    ~GLLogger();

    void report( const std::string& message );
    void warn( const std::string& message );
    void error( const std::string& message );

private:
    std::ofstream _file;
};


}
}


#endif
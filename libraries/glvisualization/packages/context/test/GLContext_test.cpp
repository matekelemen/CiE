#include "catch.hpp"
#include "../inc/GLContext.hpp"
#include "../inc/DrawManager.hpp"
#include <iostream>


cie::gl::GLContext context_global( 4,5,4,"glvisualization_testrunner_log.txt" );


namespace cie {
namespace gl {

/*
TEST_CASE( "GLContext" )
{

    auto loopFactory = [] (cie::gl::GLContext& context) -> cie::gl::EventLoopFunction
    {
        int* loopCounter = new int(0);

        auto function = [&context, loopCounter]() -> void
        {
            context.log( std::to_string(*loopCounter) );
            if (++(*loopCounter) > 8 )
            {
                delete loopCounter;
                glfwSetWindowShouldClose( context.window(), 1 );
            }
        };

        return function;
    };

    REQUIRE_NOTHROW( context_global.openWindow() );
    REQUIRE_NOTHROW( context_global.makeContextCurrent() );
    REQUIRE_NOTHROW( context_global.startEventLoop( loopFactory ) );
}
*/




TEST_CASE( "DrawManager" )
{
    // Create context
    REQUIRE_NOTHROW( context_global.openWindow() );
    REQUIRE_NOTHROW( context_global.makeContextCurrent() );
    
    // Create draw manager
    DrawManager drawManager( context_global );
    REQUIRE_NOTHROW(drawManager.makeProgram());
    REQUIRE_NOTHROW(drawManager.initialize());

    // Start the event loop
    auto drawFunctionFactory = [&drawManager](GLContext& context_global)->EventLoopFunction 
    {
        auto function = [&drawManager] () ->void { drawManager.draw(); };
        return function;
    };
    REQUIRE_NOTHROW(context_global.startEventLoop( drawFunctionFactory ));
}



/*
// Shader sources
const GLchar* vertexSource = R"glsl(
    #version 150 core
    in vec2 position;
    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";
const GLchar* fragmentSource = R"glsl(
    #version 150 core
    out vec4 outColor;
    void main()
    {
        outColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
)glsl";

TEST_CASE("main")
{
    REQUIRE_NOTHROW( context_global.openWindow() );
    REQUIRE_NOTHROW( context_global.makeContextCurrent() );

    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    std::cout << "--- MARK ---\n";
    glBindVertexArray(vao);


    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLfloat vertices[] = {
         0.0f,  0.5f,
         0.5f, -0.5f,
        -0.5f, -0.5f
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    bool running = true;
    while (running)
    {
        glfwPollEvents();

        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw a triangle from the 3 vertices
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers
        glfwSwapBuffers( context_global.window() );
    }

    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);
}
*/



}
}
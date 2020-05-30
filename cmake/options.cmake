# ---------------------------------------------------------
# GENERAL OPTIONS
# ---------------------------------------------------------
# What to install
OPTION( CIE_INSTALL_TESTS "Install unit-and system tests" ON )

# ---------------------------------------------------------
# OPENGL OPTIONS
# ---------------------------------------------------------
OPTION( CIE_USE_CPP_GRAPHICS "Compile OpenGL and GLEW" ON )
set( OpenGL_GL_PREFERENCE "GLVND" CACHE STRING "GLVND or LEGACY" )
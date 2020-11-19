// --- Graphics Includes ---
#include <ciegl/ciegl.hpp>

// --- Utility Includes ---
#include <cieutils/logging.hpp>
#include <cieutils/commandline.hpp>
#include <cieutils/macros.hpp>
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <filesystem>


namespace cie {


int main( int argc, char const* argv[] )
{
    // Set up logging
    utils::Logger log( INSTALL_PATH + "/STLConverter.log", true );

    // Parse command line arguments
    utils::CommandLineArguments args( argc, argv );

    // Parse and write loop
    for ( const auto& r_filePath : args.arguments() )
    {
        auto localBlock = log.newBlock( r_filePath );

        std::filesystem::path filePath( r_filePath );

        // Check whether file exists
        if ( !std::filesystem::exists( filePath ) )
            CIE_THROW( Exception, "File not found: " + r_filePath )

        // Parse file - supported formats:
        //  .stl
        //  .obj
        gl::PartPtr p_model;
        auto extension = filePath.extension();

        if ( extension == ".stl" )
            p_model.reset( new gl::STLPart(filePath) );
        else if ( extension == ".obj" )
            p_model.reset( new gl::ObjPart(filePath) );
        else
            CIE_THROW( Exception, "Unsuppoerted file extension: " + std::string(extension) )
        

        // Write file
        auto outputFileName = filePath.replace_extension( ".stl" );
        log.log( "Write STL to " + std::string(outputFileName) );
        p_model->writeSTL( outputFileName );
    }
}


} // namespace cie




int main( int argc, char const* argv[] )
{
    return cie::main( argc, argv );
}
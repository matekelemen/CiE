#ifndef CIE_GL_OBJ_PART_HPP
#define CIE_GL_OBJ_PART_HPP

// --- Internal Includes ---
#include "ciegl/packages/file/inc/Part.hpp"


namespace cie::gl {


/**
 * Load geometry from an .obj file
 * 
 * Note: vertex positions are the only attributes loaded
 */
class ObjPart final : public Part
{
public:
    static const Size dimension = 3;
    static const Size primitive_byte_size = 3 * ObjPart::dimension * sizeof( ObjPart::data_type );

public:
    ObjPart( std::istream& r_stream );
    ObjPart( const std::filesystem::path& r_filePath );

    void load( std::istream& r_stream ) override;
};


} // namespace cie::gl


#endif
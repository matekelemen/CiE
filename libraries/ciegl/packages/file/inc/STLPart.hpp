#ifndef CIE_GL_STL_PART_HPP
#define CIE_GL_STL_PART_HPP

// --- Utility Includes ---
#include "cieutils/packages/types/inc/types.hpp"

// --- Internal Includes ---
#include "ciegl/packages/file/inc/Part.hpp"

// --- STL Includes ---
#include <string>
#include <fstream>


namespace cie::gl {


class STLPart : public Part<float>
{
public:
    static const Size dimension = 3;

    /**
     * Normal + 3 points of a triangle,
     * all stored as 32-bit single-precision floats
     */
    static const Size primitive_byte_size = 4 * dimension * sizeof(typename STLPart::data_type);

public:
    STLPart( std::istream& r_stream );
    STLPart( const std::string& r_fileName );
    STLPart() = default;

    void load( std::istream& r_stream );
};


} // namespace cie::gl


#endif
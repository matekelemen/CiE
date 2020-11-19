#ifndef CIE_GL_STL_PART_HPP
#define CIE_GL_STL_PART_HPP

// --- Internal Includes ---
#include "ciegl/packages/file/inc/Part.hpp"

// --- STL Includes ---
#include <string>


namespace cie::gl {


class STLPart final : public Part
{
public:
    static const Size dimension = 3;

    /**
     * Coordinates for all vertices of the triangle,
     * all stored as 32-bit single-precision floats
     */
    static const Size primitive_byte_size = 3 * dimension * sizeof(typename STLPart::data_type);

public:
    STLPart( std::istream& r_stream );
    STLPart( const std::string& r_fileName );

    void load( std::istream& r_stream ) override;

    /**
     * Check whether the order of triangle vertices is consistent
     * with the corresponding normal and permute them if they are not.
     * 
     * Note: this function supports 3D models only
     */
    void repairOrientation();
};


using STLPartPtr = std::shared_ptr<STLPart>;


} // namespace cie::gl


#endif
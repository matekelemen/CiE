#ifndef CIE_GL_PART_HPP
#define CIE_GL_PART_HPP

// --- Utility Includes ---
#include "cieutils/packages/types/inc/types.hpp"

// --- STL Includes ---
#include <vector>
#include <memory>
#include <istream>
#include <filesystem>


namespace cie::gl {


class Part {
public:
    using data_type      = float;
    using data_container = std::vector<data_type>;

public:
    Part( Size dimension,
          Size primitiveByteSize );

    Part( Size dimension,
          Size primitiveByteSize,
          std::istream& r_stream );

    /**
     * Total size of all attributes in bytes
     */
    Size byteCount() const;

    /**
     * Number of stored primitives
     * (triangles most likely, but depends on the subclass)
     */
    Size numberOfPrimitives() const;

    /**
     * Load data from input stream
     */
    virtual void load( std::istream& r_stream );

    /**
     * Write in STL format
     * 
     * Note: 3D triangles only
     */
    virtual void writeSTL( const std::filesystem::path& r_path ) const;

    const data_container& data() const;

protected:
    data_container _data;
    Size           _dimension;
    Size           _primitiveByteSize;  // Number of bytes required to store all attributes of a primitive
};


using PartPtr = std::shared_ptr<Part>;


} // namespace cie::gl


#endif
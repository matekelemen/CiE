#ifndef CIE_CSG_PARTITIONING_BOX_FILE_HPP
#define CIE_CSG_PARTITIONING_BOX_FILE_HPP

// --- Utility Includes ---
#include "cieutils/packages/output/inc/FileManager.hpp"

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/Cube.hpp"
#include "CSG/packages/primitives/inc/Box.hpp"
#include "CSG/packages/primitives/inc/concepts.hpp"

// --- STL Includes ---
#include <fstream>
#include <filesystem>
#include <string>


namespace cie::csg {


/**
 * .boxes
 * 
 * 1) uint64_t dimension
 * 2) uint64_t size of coordinate_type in bytes
 * 3) set of boxes:     boxMin_0 boxMin_1 ... boxMin_<dimension-1>
 *                      boxMax_0 boxMax_1 ... boxMax_<dimension-1>
 */

class BoxFile
{
public:
    BoxFile( const std::filesystem::path& r_path,
             Size dimension = 0,
             Size coordinateByteSize = 0 );

    /* OUTPUT */

    template <concepts::Cube PrimitiveType>
    friend BoxFile& operator<<( BoxFile& r_stream, const PrimitiveType& r_cube );

    template <concepts::Box PrimitiveType>
    friend BoxFile& operator<<( BoxFile& r_stream, const PrimitiveType& r_box );

    template <concepts::PrimitiveContainer ContainerType>
    friend BoxFile& operator<<( BoxFile& r_stream, const ContainerType& r_primitives );

    template <concepts::PrimitivePtrContainer ContainerType>
    friend BoxFile& operator<<( BoxFile& r_stream, const ContainerType& r_primitives );

    /* INPUT */

    template <concepts::Box PrimitiveType>
    friend BoxFile& operator>>( BoxFile& r_stream, PrimitiveType& r_box );

    template <concepts::PrimitiveContainer ContainerType>
    friend BoxFile& operator>>( BoxFile& r_stream, ContainerType& r_primitives );

    template <concepts::PrimitivePtrContainer ContainerType>
    friend BoxFile& operator>>( BoxFile& r_stream, ContainerType& r_primitives );

private:
    const std::string     _extension;
    Size                  _dimension;
    Size                  _coordinateByteSize;
    utils::FileManager    _fileManager;
    utils::FilePtr        _p_file;
};


} // namespace cie::csg

#include "CSG/packages/io/impl/BoxFile_impl.hpp"

#endif
#ifndef CIE_CSG_TREES_WRITE_HPP
#define CIE_CSG_TREES_WRITE_HPP

// --- External Includes ---
#include "pugixml.hpp"

// --- Internal Includes ---
#include "CSG/packages/trees/inc/spacetreenode.hpp"


namespace cie::csg {

/**
 * TODO: lots of things; doesn't support everything that can be 
 * instantiated right now.
*/
template <class NodeType>
void writeToVTK( NodeType& node,
                 const std::string& fileName );


} // namespace cie::csg

#include "CSG/packages/trees/impl/write_impl.hpp"

#endif
// --- Utility Includes ---
#include "cieutils/packages/macros/inc/checks.hpp"

// --- Internal Includes ---
#include "ciegl/packages/file/inc/Part.hpp"

// --- STL Includes ---
#include <fstream>


namespace cie::gl {


Part::Part() :
    _data(),
    _indices()
{
}


Size Part::attributeByteCount() const
{
    return this->_data.size() * sizeof( Part::data_type );
}


Size Part::indexByteCount() const
{
    return this->_indices.size() * sizeof( Part::index_type );
}


Size Part::numberOfVertices() const
{
    return this->_data.size() / this->vertexAttributeSize();
}


Size Part::numberOfPrimitives() const
{
    return this->_indices.size() / this->primitiveAttributeSize();
}


void Part::load( std::istream& r_stream )
{
    CIE_THROW( AbstractCallException, "Call to pure virtual Part::load" )
}


const Part::data_container& Part::data() const
{
    return this->_data;
}


const Part::index_container& Part::indices() const
{
    return this->_indices;
}


Size Part::primitiveAttributeSize() const
{
    return this->vertexAttributeSize() * this->primitiveVertexSize();
}


Size Part::primitiveByteSize() const
{
    return this->primitiveAttributeSize() * sizeof( Part::data_type );
}


} // namespace cie::gl
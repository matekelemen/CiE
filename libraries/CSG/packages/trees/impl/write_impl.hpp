#ifndef CIE_CSG_TREES_WRITE_IMPL_HPP
#define CIE_CSG_TREES_WRITE_IMPL_HPP

// --- STL Includes ---
#include <sstream>


namespace cie::csg {


template <class NodeType>
void writeToVTK( NodeType& r_node,
                 const std::filesystem::path& r_filePath )
{
    // Init
    const Size dimension = NodeType::dimension;

    // Header
    pugi::xml_document document;

    auto head = document.append_child( "VTKFile" );
    head.append_attribute( "type" ).set_value( "UnstructuredGrid" );
    head.append_attribute( "version" ).set_value( "0.1" );
    head.append_attribute( "byte_order" ).set_value( "LittleEndian" );
    //head.append_attribute( "compressor" ).set_value( "vtkZLibDataCompressor" );

    auto grid           = head.append_child( "UnstructuredGrid" );
    auto piece          = grid.append_child( "Piece" );
    Size numberOfPoints = 0;
    Size numberOfNodes  = 0;
    std::string indent;

    // Points
    auto points         = piece.append_child( "Points" ).append_child( "DataArray" );
    points.append_attribute( "type" ).set_value( "Float64" );
    points.append_attribute( "NumberOfComponents" ).set_value( 3 ); // <-- TODO: Paraview bug? Looks like points have to be 3D
    points.append_attribute( "format" ).set_value( "ascii" );

    auto nodeData       = points.append_child( pugi::node_pcdata );
    indent              = std::string( 5, '\t' );
    std::stringstream stringStream, valueStream, levelStream;
    stringStream << std::scientific;
    stringStream << "\n" << indent;
    valueStream << "\n" << indent;
    levelStream << "\n" << indent;

    auto package = [&stringStream, &valueStream, &levelStream, &numberOfPoints, &numberOfNodes, &indent]( NodeType* p_node ) -> bool
    {
        if ( p_node->values().empty() )
            return true;

        Size numberOfSamplePoints = p_node->sampler()->size();
        //numberOfNodes++;
        numberOfPoints += numberOfSamplePoints;

        typename NodeType::sample_point_iterator it_point(0,*p_node);
        for ( Size pointIndex=0; pointIndex<numberOfSamplePoints; ++pointIndex,++it_point )
        {
            // First coordinate
            auto point = *it_point;
            stringStream << point[0];
            Size i = 1;

            // Rest of the coordinates
            for ( ; i<dimension; ++i)
                stringStream << " " << point[i];

            // Pad with zeros because Paraview
            while ( i++ < 3 )
                stringStream << " " << 0.0;

            stringStream << "\n" << indent;
        }

        // Get level and values
        Size level = p_node->level();

        for ( const auto& value : p_node->values() )
        {
            valueStream << value << "\n" << indent;
            levelStream << level << "\n" << indent;
        }


        return true;
    };

    // Collect points and values
    r_node.visit( package );
    nodeData.set_value( stringStream.str().c_str() );

    stringStream.clear();

    // Cells - TODO
    auto cells = piece.append_child( "Cells" ).append_child( "DataArray" );
    cells.append_attribute( "type" ).set_value( "UInt64" );
    cells.append_attribute( "Name" ).set_value( "connectivity" );
    cells.append_attribute( "format" ).set_value( "ascii" );

    // Point data
    auto pointData = piece.append_child( "PointData" ).append_child( "DataArray" );
    pointData.append_attribute( "type" ).set_value( "Float64" );
    pointData.append_attribute( "Name" ).set_value( "Target" );
    pointData.append_attribute( "NumberOfComponents" ).set_value( "1" );
    pointData.append_attribute( "format" ).set_value( "ascii" );

    pointData.append_child( pugi::node_pcdata ).set_value( valueStream.str().c_str() );
    valueStream.clear();

    // Level data
    auto levelData = piece.append_child( "PointData" ).append_child( "DataArray" );
    levelData.append_attribute( "type" ).set_value( "Float64" );
    levelData.append_attribute( "Name" ).set_value( "Level" );
    levelData.append_attribute( "NumberOfComponents" ).set_value( "1" );
    levelData.append_attribute( "format" ).set_value( "ascii" );

    levelData.append_child( pugi::node_pcdata ).set_value( levelStream.str().c_str() );
    levelStream.clear();

    // Fill in missing info
    piece.append_attribute( "NumberOfPoints" ).set_value( numberOfPoints );
    piece.append_attribute( "NumberOfCells" ).set_value( numberOfNodes );

    // Write to file
    document.save_file( r_filePath.c_str() );
}


} // namespace cie::csg


#endif
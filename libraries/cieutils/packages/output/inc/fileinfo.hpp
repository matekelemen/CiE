#ifndef CIE_FILEINFO_HPP
#define CIE_FILEINFO_HPP

// --- STL Includes ---
#include <string>
#include <vector>


namespace cie {
namespace utils {
namespace detail {


const std::vector<std::string> textExtensions = {   "txt",
                                                    "csv"   };
const std::vector<std::string> binaryExtensions = { "bin",
                                                    "b"     };



const std::string fileExtension( const std::string& fileName );
const std::string fileName( const std::string& fileName );
const std::string fileDirectory( const std::string& fileName );

bool isTextExtension( const std::string& fileName );
bool isBinaryExtension( const std::string& fileName );

bool isFile( const std::string& fileName );
bool isTextFile( const std::string& fileName );
bool isBinaryFile( const std::string& fileName );
bool isDirectory( const std::string& fileName );


}
}
}

#endif
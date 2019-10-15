#include "csgtypes.hpp"

namespace csg {

void writePointVector(const std::string& filename, const std::vector<Point64>& data){
    std::ofstream file(filename);
    if (file.is_open()){
        for (auto it=data.begin();it!=data.end();++it){
            for (uint j=0;j<it->size();++j){
                file << std::to_string((*it)[j]) +',';
            }
            file <<"\n";
        }
        file.close();
    }
    else{
        throw std::runtime_error("Failed to open " + filename);
    }
}

}

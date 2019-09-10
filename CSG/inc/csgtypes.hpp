#ifndef CSG_CSGTYPES_HPP
#define CSG_CSGTYPES_HPP

#include <array>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

namespace csg{

// -----------------------------------------------------------
// POINT
// -----------------------------------------------------------
template <typename dType> class Point {
private:
    std::array<dType, 3> data_;
public:
    Point(dType x, dType y, dType z)    : data_({x,y,z}){}
    Point(dType x, dType y)             : Point(x,y,0.0){}
    Point(dType x)                      : Point(x,0.0,0.0){}
    Point()                             : Point(0.0,0.0,0.0){}
    Point<dType>(const Point<dType>& point) {
        for (uint i=0; i<this->size(); ++i){
            data_[i] = point[i];
        }
    }

    dType& operator[](uint index) { 
        if (index<this->size()) return data_[index];
        else throw std::runtime_error("Point index out of bounds!");
    }

    const dType& operator[](uint index) const { 
        if (index<this->size()) return data_[index];
        else throw std::runtime_error("Point index out of bounds!");
    }

    Point<dType> operator=(const Point<dType>& point){
        return Point<dType>(point);
    }

    uint size() const {return data_.size();}
};


typedef Point<double>   Point64;
typedef Point<float>    Point32;
typedef Point<int>      PointInt;


// -----------------------------------------------------------
// TYPEDEFS
// -----------------------------------------------------------
typedef std::array<bool,25>     NodeData;
typedef std::array<Point64,2>   BoundingBox;

// -----------------------------------------------------------
// MISC FUNCTIONS
// -----------------------------------------------------------
template <typename num>
bool sgn(num number) {return (number>=0 ? true : false);}

// -----------------------------------------------------------
// WRITE FUNCTIONS
// -----------------------------------------------------------
void writePointVector(const std::string& filename, const std::vector<Point64>& data);

}
#endif
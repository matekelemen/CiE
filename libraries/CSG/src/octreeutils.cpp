#include "octreeutils.hpp"

namespace csg {





std::array<bool,3> base2(uint index){
    std::array<bool,3> output({false,false,false});
    if (index/4 == 1) {
        output[2]   = true;
        index       -= 4;
    }
    if (index/2 == 1){
        output[1]   = true;
        index       -= 2;
    }
    if (index == 1) output[0] = true;

    return output;
}

std::array<uint,3> base3(uint index){
    std::array<uint,3> output({0,0,0});
    uint exp;
    for (uint i=0;i<3;++i){
        exp         = uintPow(3,2-i);
        output[2-i] = index / exp;
        index       -= exp*output[2-i];
    }

    return output;
}


uint base10(const std::array<bool,3>& index){
    uint index10 = 0;
    if (index[0]) index10+=1;
    if (index[1]) index10+=2;
    if (index[2]) index10+=4;
    return index10;
}


uint base10(const std::array<uint,3>& index){
    return index[0] + 3*index[1] + 9*index[2];
}


uint uintPow(uint base, uint exponent){
    if (exponent==0) return 1;
    if (exponent==1) return base;
    uint output = uintPow(base,exponent/2);
    if (exponent%2 == 0) return output*output;
    else return base*output*output;
}


}
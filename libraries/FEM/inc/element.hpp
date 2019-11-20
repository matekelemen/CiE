#ifndef FEM_ELEMENT_HPP
#define FEM_ELEMENT_HPP

#include "linalg.hpp"


namespace cie {
namespace fem {


typedef cie::linalg::Matrix Matrix;


class Element {
public:
	virtual Matrix& MaterialStiffness() const { return Matrix; }
	virtual Matrix& GeometricStiffness() const { return Matrix; }
};


}
}

#endif
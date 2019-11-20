#ifndef CSG_CSGOPERATOR_HPP
#define CSG_CSGOPERATOR_HPP

#include "csgobject.hpp"

namespace cie {
namespace csg{

class CSGOperator : public CSGObject {
protected:
    CSGObject* left_;
    CSGObject* right_;
public:
    // Constructors/Destructor
    CSGOperator(){}
    CSGOperator(const CSGOperator& op);
    CSGOperator(CSGObject* left, CSGObject* right);
    ~CSGOperator();
    // Override
    virtual bool isInside(const Point64& point) const override;
};



class CSGUnion : public CSGOperator {
public:
    CSGUnion(CSGObject* left, CSGObject* right) : CSGOperator(left,right){}
    bool isInside(const Point64& point) const override;
};

class CSGSubtract : public CSGOperator {
public:
    CSGSubtract(CSGObject* left, CSGObject* right) : CSGOperator(left,right){}
    bool isInside(const Point64& point) const override;
};

class CSGIntersect : public CSGOperator {
public:
    CSGIntersect(CSGObject* left, CSGObject* right) : CSGOperator(left,right){}
    bool isInside(const Point64& point) const override;
};

}
}
#endif
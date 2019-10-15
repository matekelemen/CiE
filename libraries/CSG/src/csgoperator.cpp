#include "csgoperator.hpp"

namespace csg {

CSGOperator::CSGOperator(CSGObject* left, CSGObject* right) :
    left_(left),
    right_(right)
{
}

CSGOperator::~CSGOperator(){
    delete left_;
    delete right_;
}

bool CSGOperator::isInside(const Point64& point) const{
    throw std::runtime_error("Calling virtual function CSGOperator::isInside");
    return false;
}


bool CSGUnion::isInside(const Point64& point) const{
    return left_->isInside(point) || right_->isInside(point);
}

bool CSGSubtract::isInside(const Point64& point) const{
    return left_->isInside(point) && !right_->isInside(point);
}

bool CSGIntersect::isInside(const Point64& point) const{
    return left_->isInside(point) && right_->isInside(point);
}




}
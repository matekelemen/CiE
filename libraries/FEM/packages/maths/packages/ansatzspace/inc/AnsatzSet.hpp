#ifndef CIE_FEM_MATHS_ANSATZ_SET_HPP
#define CIE_FEM_MATHS_ANSATZ_SET_HPP

// --- Internal Includes ---
#include "FEM/packages/maths/inc/ScalarFunction.hpp"

// --- STL Includes ---
#include <vector>
#include <memory>


namespace cie::fem::maths {


template <Size Dimension, concepts::NumericType NT>
class AnsatzSet
{
public:
    using ansatz_function = ScalarFunction<Dimension,NT>;
    using ansatz_ptr = std::shared_ptr<ansatz_function>;
    using ansatz_container = std::vector<ansatz_ptr>;

public:
    AnsatzSet( ansatz_container&& r_ansatzFunctions );

    AnsatzSet( const ansatz_container& r_ansatzFunctions );

    AnsatzSet() = default;

    AnsatzSet( AnsatzSet<Dimension,NT>&& r_rhs ) = default;

    AnsatzSet( const AnsatzSet<Dimension,NT>& r_rhs ) = default;

    AnsatzSet<Dimension,NT>& operator=( const AnsatzSet<Dimension,NT>& r_rhs ) = default;

    Size size() const;

    const ansatz_function& operator[]( Size index ) const;

    const ansatz_function& at( Size index ) const;

    void push_back( const ansatz_ptr& rp_ansatzFunction );

    template <class AnsatzFunction, class ...Args>
    requires concepts::DerivedFrom<AnsatzFunction,ansatz_function>
    void emplace_back( Args&&... r_args );

protected:
    ansatz_container _ansatzFunctions;
};


} // namespace cie::fem::maths

#include "FEM/packages/maths/packages/ansatzspace/impl/AnsatzSet_impl.hpp"

#endif
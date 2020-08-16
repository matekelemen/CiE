#ifndef CIE_FEM_DOF_MAP_HPP
#define CIE_FEM_DOF_MAP_HPP

// --- Utility Includes ---
#include <cieutils/concepts.hpp>
#include <cieutils/types.hpp>


namespace cie::fem {


template <concepts::NumericContainer ContainerType>
class DoFMap
{
public:
    typedef ContainerType   dof_container;

public:
    DoFMap( const dof_container& dofs ) : _dofs(dofs)   {}
    Size dof( Size index ) const                        { return _dofs[index]; }
    Size size() const                                   { return _dofs.size(); }
    const dof_container& dofs() const                   { return _dofs; }

protected:
    ContainerType   _dofs;
};


} // namespace cie::fem

#endif
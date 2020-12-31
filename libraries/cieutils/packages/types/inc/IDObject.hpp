#ifndef CIE_UTILS_ID_OBJECT_HPP
#define CIE_UTILS_ID_OBJECT_HPP

// --- Internal Includes ---
#include "cieutils/packages/types/inc/types.hpp"


namespace cie::utils {


template <class IDType = Size>
class IDObject
{
public:
    using id_type   = IDType;

public:
    IDObject( const IDType& r_id );
    IDObject();
    IDObject( const IDObject<IDType>& r_rhs );
    IDObject<IDType>& operator=( const IDObject<IDType>& rhs );

    virtual void setID( const IDType& id );
    virtual const IDType& getID() const;

protected:
    IDType _id;
};


} // namespace cie::utils

#include "cieutils/packages/types/impl/IDObject_impl.hpp"

#endif
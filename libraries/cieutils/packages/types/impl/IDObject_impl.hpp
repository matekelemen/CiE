#ifndef CIE_UTILS_ID_OBJECT_IMPL_HPP
#define CIE_UTILS_ID_OBJECT_IMPL_HPP


namespace cie::utils {


template <class IDType>
IDObject<IDType>::IDObject( const IDType& r_id ) :
    _id(r_id)
{
}


template <class IDType>
IDObject<IDType>::IDObject() :
    IDObject<IDType>( 0 )
{
}


template <class IDType>
IDObject<IDType>::IDObject( const IDObject<IDType>& r_rhs ) :
    _id( r_rhs._id )
{
}


template <class IDType>
inline IDObject<IDType>&
IDObject<IDType>::operator=( const IDObject<IDType>& r_rhs )
{
    _id = r_rhs._id;
    return *this;
}


template <class IDType>
inline void
IDObject<IDType>::setID( const IDType& id )
{
    _id = id;
}


template <class IDType>
inline const IDType&
IDObject<IDType>::getID() const
{
    return _id;
}


} // namespace cie::utils


#endif
#ifndef CIE_GL_PART_IMPL_HPP
#define CIE_GL_PART_IMPL_HPP


namespace cie::gl {


template <concepts::NumericType NT>
Part<NT>::Part() :
    _data()
{
}


template <concepts::NumericType NT>
inline typename Part<NT>::data_container&
Part<NT>::data()
{
    return this->_data;
}


template <concepts::NumericType NT>
inline const typename Part<NT>::data_container&
Part<NT>::data() const
{
    return this->_data;
}


} // namespace cie::gl


#endif
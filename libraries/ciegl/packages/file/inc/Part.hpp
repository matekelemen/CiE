#ifndef CIE_GL_PART_HPP
#define CIE_GL_PART_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"

// --- STL Includes ---
#include <vector>


namespace cie::gl {


template <concepts::NumericType NT>
class Part {
public:
    using data_type      = NT;
    using data_container = std::vector<NT>;

public:
    Part();

    data_container& data();
    const data_container& data() const;

protected:
    data_container _data;
};


} // namespace cie::gl

#include "ciegl/packages/file/impl/Part_impl.hpp"

#endif
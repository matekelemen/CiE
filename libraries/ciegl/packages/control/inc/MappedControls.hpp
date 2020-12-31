#ifndef CIE_GL_MAPPED_CONTROLS_HPP
#define CIE_GL_MAPPED_CONTROLS_HPP

// --- Internal Includes ---
#include "ciegl/packages/control/inc/ControlKey.hpp"
#include "ciegl/packages/control/inc/ControlKey.hpp"

// --- STL Includes ---
#include <unordered_map>
#include <deque>
#include <string>
#include <functional>
#include <istream>


namespace cie::gl {


class MappedControls
{
public:
    using control_function       = std::function<void()>;
    using map_type               = std::unordered_map<KeyEnum,control_function>;
    using configuration_map_type = std::unordered_map<ControlKey,control_function>;
    using configuration_contents = std::deque<std::pair<std::string,ControlKey>>;

public:
    MappedControls();

    void configure( configuration_map_type& r_configMap );
    
    /**
     * Load control configuration from an xml file with the following layout:
     * <controls>
     *  <c0 name="control_name">
     *      <key> "key_char" </key>
     *  </c0>
     *  .
     *  .
     *  .
     * </controls>
     */
    void configure( std::istream& r_configurationFile );

    void callControl( KeyEnum key );

protected:
    virtual configuration_map_type makeConfigurationMap( configuration_contents& r_configContents ) = 0;

protected:
    map_type _controlMap;
};


} // namespace cie::gl

#include "ciegl/packages/control/impl/MappedControls_impl.hpp"

#endif
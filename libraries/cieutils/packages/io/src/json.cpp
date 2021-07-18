// --- External Includes ---
#include "nlohmann/json.hpp"

// --- Utiltity Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "cieutils/packages/io/inc/json.hpp"

// --- STL Includes ---
#include <fstream>
#include <vector>
#include <array>
#include <iostream>


namespace cie::io {


// ----------------------------------------------------------
// MEMBER TEMPLATE HELPERS
// ----------------------------------------------------------

template <class ValueType>
struct SetGetTemplate
{
    static void set( nlohmann::json& r_json,
                     const ValueType& r_value )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        r_json = r_value;

        CIE_END_EXCEPTION_TRACING
    }

    static ValueType as( const nlohmann::json& r_json )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        return r_json.get<ValueType>();

        CIE_END_EXCEPTION_TRACING
    }
};


template <>
struct SetGetTemplate<JSONObject>
{
    static void set( nlohmann::json& r_json,
                     const JSONObject& r_value )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        r_json = r_value.contents();

        CIE_END_EXCEPTION_TRACING
    }

    static JSONObject as( const nlohmann::json& r_json )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        return JSONObject( r_json.dump() );

        CIE_END_EXCEPTION_TRACING
    }
};


template <concepts::io::SupportedType ValueType>
struct TypeQueryTemplate
{
};


template <>
struct TypeQueryTemplate<bool>
{
    static bool is( const JSONObject& r_json )
    { return r_json.contents().is_boolean(); }

    static JSONObject addDefault( JSONObject& r_json,
                                  JSONObject::content_type& r_contents,
                                  const std::string& r_key )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        r_contents[r_key] = false;
        return JSONObject(
            &r_contents[r_key],
            &r_json.root()
        );

        CIE_END_EXCEPTION_TRACING
    }
};


template <>
struct TypeQueryTemplate<int>
{
    static bool is( const JSONObject& r_json )
    { return r_json.contents().is_number_integer(); }

    static JSONObject addDefault( JSONObject& r_json,
                                  JSONObject::content_type& r_contents,
                                  const std::string& r_key )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        r_contents[r_key] = 0;
        return JSONObject(
            &r_contents[r_key],
            &r_json.root()
        );

        CIE_END_EXCEPTION_TRACING
    }
};


template <>
struct TypeQueryTemplate<Size>
{
    static bool is( const JSONObject& r_json )
    { return r_json.contents().is_number_unsigned() && r_json.contents().is_number_integer(); }

    static JSONObject addDefault( JSONObject& r_json,
                                  JSONObject::content_type& r_contents,
                                  const std::string& r_key )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        r_contents[r_key] = 0;
        return JSONObject(
            &r_contents[r_key],
            &r_json.root()
        );

        CIE_END_EXCEPTION_TRACING
    }
};


template <>
struct TypeQueryTemplate<float>
{
    static bool is( const JSONObject& r_json )
    { return r_json.contents().is_number_float(); }

    static JSONObject addDefault( JSONObject& r_json,
                                  JSONObject::content_type& r_contents,
                                  const std::string& r_key )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        r_contents[r_key] = 0;
        return JSONObject(
            &r_contents[r_key],
            &r_json.root()
        );

        CIE_END_EXCEPTION_TRACING
    }
};


template <>
struct TypeQueryTemplate<double>
{
    static bool is( const JSONObject& r_json )
    { return r_json.contents().is_number_float(); }

    static JSONObject addDefault( JSONObject& r_json,
                                  JSONObject::content_type& r_contents,
                                  const std::string& r_key )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        r_contents[r_key] = 0;
        return JSONObject(
            &r_contents[r_key],
            &r_json.root()
        );

        CIE_END_EXCEPTION_TRACING
    }
};


template <>
struct TypeQueryTemplate<std::string>
{
    static bool is( const JSONObject& r_json )
    { return r_json.contents().is_string(); }

    static JSONObject addDefault( JSONObject& r_json,
                                  JSONObject::content_type& r_contents,
                                  const std::string& r_key )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        r_contents[r_key] = "";
        return JSONObject(
            &r_contents[r_key],
            &r_json.root()
        );

        CIE_END_EXCEPTION_TRACING
    }
};


template <>
struct TypeQueryTemplate<JSONObject>
{
    static bool is( const JSONObject& r_json )
    { return r_json.isObject(); }

    static JSONObject addDefault( JSONObject& r_json,
                                  JSONObject::content_type& r_contents,
                                  const std::string& r_key )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        r_contents[r_key] = {};
        return JSONObject(
            &r_contents[r_key],
            &r_json.root()
        );

        CIE_END_EXCEPTION_TRACING
    }
};


// ----------------------------------------------------------
// ARRAY MEMBER TEMPLATE HELPERS
// ----------------------------------------------------------

// Helper for array assignments
// WARNING: an adequately preallocated contiguous array is required
template <class ValueType>
struct AssignToContiguousArray
{
    static void as( const nlohmann::json& r_json,
                    ValueType* p_begin )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        for ( const auto& r_component : r_json )
            *p_begin++ = SetGetTemplate<ValueType>::as( r_component );

        CIE_END_EXCEPTION_TRACING
    }
};


// Helper for std::vector<ValueType>
template <class ValueType>
struct SetGetTemplate<std::vector<ValueType>>
{
    static void set( nlohmann::json& r_json,
                     const std::vector<ValueType>& r_value )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        r_json.clear();

        for ( const auto& r_component : r_value )
            r_json.push_back( r_component );

        CIE_END_EXCEPTION_TRACING
    }

    static std::vector<ValueType> as( const nlohmann::json& r_json )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        std::vector<ValueType> output;
        output.resize( r_json.size() );

        if ( !output.empty() )
            AssignToContiguousArray<ValueType>::as( r_json, &output[0] );

        return output;

        CIE_END_EXCEPTION_TRACING
    }
};


// Helper for std::vector<JSONObject>
template <>
struct SetGetTemplate<std::vector<JSONObject>>
{
    static void set( nlohmann::json& r_json,
                     const std::vector<JSONObject>& r_value )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        r_json.clear();

        for ( const auto& r_component : r_value )
            r_json.push_back( r_component.contents() );

        CIE_END_EXCEPTION_TRACING
    }

    static std::vector<JSONObject> as( const nlohmann::json& r_json )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        std::vector<JSONObject> output;
        output.resize( r_json.size() );

        if ( !output.empty() )
            AssignToContiguousArray<JSONObject>::as( r_json, &output[0] );

        return output;

        CIE_END_EXCEPTION_TRACING
    }
};


// Helper for std::array<ValueType,ArraySize>
template <class ValueType, Size ArraySize>
struct SetGetTemplate<std::array<ValueType,ArraySize>>
{
    static void set( nlohmann::json& r_json,
                     const std::array<ValueType,ArraySize>& r_value )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        r_json.clear();

        for ( const auto& r_component : r_value )
            r_json.push_back( r_component );

        CIE_END_EXCEPTION_TRACING
    }

    static std::array<ValueType,ArraySize> as( const nlohmann::json& r_json )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        CIE_CHECK(
            r_json.size() == ArraySize,
            "destination size (" + std::to_string(ArraySize) + ") does not match source size (" + std::to_string(r_json.size()) + ")"
        )

        std::array<ValueType,ArraySize> output;
        AssignToContiguousArray<ValueType>::as( r_json, &output[0] );

        return output;

        CIE_END_EXCEPTION_TRACING
    }
};


// Helper for std::array<JSONObject,ArraySize>
template <Size ArraySize>
struct SetGetTemplate<std::array<JSONObject,ArraySize>>
{
    static void set( nlohmann::json& r_json,
                     const std::array<JSONObject,ArraySize>& r_value )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        r_json.clear();

        for ( const auto& r_component : r_value )
            r_json.push_back( r_component.contents() );

        CIE_END_EXCEPTION_TRACING
    }

    static std::array<JSONObject,ArraySize> as( const nlohmann::json& r_json )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        CIE_CHECK(
            r_json.size() == ArraySize,
            "destination size (" + std::to_string(ArraySize) + ") does not match source size (" + std::to_string(r_json.size()) + ")"
        )

        std::array<JSONObject,ArraySize> output;
        AssignToContiguousArray<JSONObject>::as( r_json, &output[0] );

        return output;

        CIE_END_EXCEPTION_TRACING
    }
};


template <class ValueType>
struct TypeQueryTemplate<std::vector<ValueType>>
{
    static bool is( const JSONObject& r_json )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        if ( !r_json.isArray() )
            return false;
        
        const Size size = r_json.size();
        for ( Size i_component=0; i_component<size; ++i_component )
            if ( !r_json[i_component].is<ValueType>() )
                return false;

        return true;

        CIE_END_EXCEPTION_TRACING
    }

    static JSONObject addDefault( JSONObject& r_json,
                                  JSONObject::content_type& r_contents,
                                  const std::string& r_key )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        r_contents[r_key] = JSONObject::content_type::array();
        return JSONObject(
            &r_contents[r_key],
            &r_json.root()
        );

        CIE_END_EXCEPTION_TRACING
    }
};


// Helper for std::array<ValueType,ArraySize>
template <class ValueType, Size ArraySize>
struct TypeQueryTemplate<std::array<ValueType,ArraySize>>
{
    static bool is( const JSONObject& r_json )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        return TypeQueryTemplate<std::vector<ValueType>>::is( r_json ) && r_json.size() == ArraySize;

        CIE_END_EXCEPTION_TRACING
    }

    static JSONObject addDefault( JSONObject& r_json,
                                  JSONObject::content_type& r_contents,
                                  const std::string& r_key )
    {
        CIE_BEGIN_EXCEPTION_TRACING

        r_contents[r_key] = JSONObject::content_type::array();
        return JSONObject(
            &r_contents[r_key],
            &r_json.root()
        );

        CIE_END_EXCEPTION_TRACING
    }
};


// ----------------------------------------------------------
// MEMBER TEMPLATE INSTANTIATIONS
// ----------------------------------------------------------

template <class ValueType>
void
JSONObject::SetGet<ValueType>::set( JSONObject& r_json,
                                    const ValueType& r_value )
{
    SetGetTemplate<ValueType>::set( 
        *r_json._p_contents,
        r_value
    );
}


template <class ValueType>
ValueType 
JSONObject::SetGet<ValueType>::as( const JSONObject& r_json )
{
    return SetGetTemplate<ValueType>::as( *r_json._p_contents );
}


template <class ValueType>
bool JSONObject::TypeQuery<ValueType>::is( const JSONObject& r_json )
{
    return TypeQueryTemplate<ValueType>::is( r_json );
}


template <class ValueType>
JSONObject JSONObject::TypeQuery<ValueType>::addDefault( JSONObject& r_json,
                                                         const std::string& r_key )
{
    return TypeQueryTemplate<ValueType>::addDefault(
        r_json,
        *r_json._p_contents.get(),
        r_key
    );
}


// Template instantiations
#define CIE_INSTANTIATE_JSON_TEMPLATE_SPECIALIZATIONS( ValueType )  \
    template struct JSONObject::SetGet<ValueType>;                  \
    template struct JSONObject::SetGet<std::vector<ValueType>>;     \
    template struct JSONObject::SetGet<std::array<ValueType,1>>;    \
    template struct JSONObject::SetGet<std::array<ValueType,2>>;    \
    template struct JSONObject::SetGet<std::array<ValueType,3>>;    \
    template struct JSONObject::TypeQuery<ValueType>;               \
    template struct JSONObject::TypeQuery<std::vector<ValueType>>;  \
    template struct JSONObject::TypeQuery<std::array<ValueType,1>>; \
    template struct JSONObject::TypeQuery<std::array<ValueType,2>>; \
    template struct JSONObject::TypeQuery<std::array<ValueType,3>>;


CIE_INSTANTIATE_JSON_TEMPLATE_SPECIALIZATIONS( Size )
CIE_INSTANTIATE_JSON_TEMPLATE_SPECIALIZATIONS( int )
CIE_INSTANTIATE_JSON_TEMPLATE_SPECIALIZATIONS( float )
CIE_INSTANTIATE_JSON_TEMPLATE_SPECIALIZATIONS( double )
CIE_INSTANTIATE_JSON_TEMPLATE_SPECIALIZATIONS( std::string )
CIE_INSTANTIATE_JSON_TEMPLATE_SPECIALIZATIONS( JSONObject )


// bool requires special treatment because of std::vector<bool> ¯\_(ツ)_/¯
template struct JSONObject::SetGet<bool>;
template struct JSONObject::SetGet<std::array<bool,1>>;
template struct JSONObject::SetGet<std::array<bool,2>>;
template struct JSONObject::SetGet<std::array<bool,3>>;
template struct JSONObject::TypeQuery<bool>;
template struct JSONObject::TypeQuery<std::array<bool,1>>;
template struct JSONObject::TypeQuery<std::array<bool,2>>;
template struct JSONObject::TypeQuery<std::array<bool,3>>;


// ----------------------------------------------------------
// CONSTRUCTOR / DESTRUCTOR
// ----------------------------------------------------------

JSONObject::JSONObject()
    : _p_contents( new nlohmann::json )
{
    _p_root = this;
}


JSONObject::JSONObject( const JSONObject& r_rhs )
    : _p_contents( new nlohmann::json(*r_rhs._p_contents) )
{
    _p_root = this;
}


JSONObject::JSONObject( JSONObject&& r_rhs )
    : _p_contents( std::move(r_rhs._p_contents) )
{
    _p_root = this;
}


JSONObject& JSONObject::operator=( const JSONObject& r_rhs )
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( _p_root == this )
        delete (JSONObject::content_type*)_p_contents;

    _p_contents = new JSONObject::content_type( *r_rhs._p_contents );
    _p_root = this;

    return *this;

    CIE_END_EXCEPTION_TRACING
}


JSONObject::JSONObject( const std::string& r_jsonString )
    : _p_contents( new JSONObject::content_type( JSONObject::content_type::parse(r_jsonString)) )
{
    _p_root = this;
}


JSONObject::JSONObject( std::string&& r_jsonString )
    : _p_contents( new JSONObject::content_type( JSONObject::content_type::parse(std::move(r_jsonString)) ) )
{
    _p_root = this;
}


JSONObject::JSONObject( const std::filesystem::path& r_filePath )
    : JSONObject()
{
    CIE_BEGIN_EXCEPTION_TRACING

    std::ifstream file( r_filePath );
    file >> *_p_contents;

    CIE_END_EXCEPTION_TRACING
}


JSONObject::JSONObject( std::istream& r_stream )
    : JSONObject()
{
    CIE_BEGIN_EXCEPTION_TRACING

    r_stream >> *_p_contents;

    CIE_END_EXCEPTION_TRACING
}


JSONObject::JSONObject( JSONObject::content_type* p_contents,
                        const JSONObject* p_root )
    : _p_contents( p_contents ),
      _p_root( p_root )
{
}


JSONObject::JSONObject( const JSONObject::content_type* p_contents,
                        const JSONObject* p_root )
    : _p_contents( p_contents ),
      _p_root( p_root )
{
}


JSONObject::~JSONObject()
{
    if ( _p_root == this )
        delete (JSONObject::content_type*)_p_contents;
}


// ----------------------------------------------------------
// NON-TEMPLATE MEMBERS
// ----------------------------------------------------------

JSONObject JSONObject::operator[]( const std::string& r_key )
{
    CIE_BEGIN_EXCEPTION_TRACING

    CIE_CHECK(
        this->hasKey(r_key),
        "'" + r_key + "' is not a key"
    )

    return JSONObject(
        &_p_contents->operator[](r_key),
        _p_root
    );

    CIE_END_EXCEPTION_TRACING
}


const JSONObject JSONObject::operator[]( const std::string& r_key ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    CIE_CHECK(
        this->hasKey(r_key),
        "'" + r_key + "' is not a key"
    )

    return JSONObject( &_p_contents->operator[](r_key), _p_root );

    CIE_END_EXCEPTION_TRACING
}


JSONObject JSONObject::operator[]( Size index )
{
    CIE_BEGIN_EXCEPTION_TRACING

    return JSONObject(
        &_p_contents->operator[]( index ),
        _p_root
    );

    CIE_END_EXCEPTION_TRACING
}


const JSONObject JSONObject::operator[]( Size index ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    return JSONObject(
        &_p_contents->operator[]( index ),
        _p_root
    );

    CIE_END_EXCEPTION_TRACING
}


bool JSONObject::hasKey( const std::string& r_key ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    return _p_contents->contains( r_key );

    CIE_END_EXCEPTION_TRACING
}


Size JSONObject::size() const
{
    return _p_contents->size();
}


bool JSONObject::isArray() const
{
    return _p_contents->is_array();
}


bool JSONObject::isObject() const
{
    return _p_contents->is_object();
}


const JSONObject::content_type& JSONObject::contents() const
{
    return *_p_contents;
}


JSONObject& JSONObject::add( const std::string& r_key,
                             const std::string& r_value,
                             bool allowOverwrite )
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( this->hasKey(r_key) )
    {
        if ( !allowOverwrite )
            CIE_THROW( Exception, "'" + r_key + "' is an existing key" )

        this->operator[]( r_key ).set( r_value );
    }
    else
    {
        this->addDefault<std::string>( r_key ).set( r_value );
    }

    return *this;

    CIE_END_EXCEPTION_TRACING
}


JSONObject& JSONObject::set( const std::string& r_value )
{
    CIE_BEGIN_EXCEPTION_TRACING

    JSONObject::SetGet<std::string>::set( *this, r_value );
    return *this;

    CIE_END_EXCEPTION_TRACING
}


const JSONObject& JSONObject::root() const
{
    CIE_CHECK_POINTER( _p_root )
    return *_p_root;
}


// ----------------------------------------------------------
// NON-MEMBERS
// ----------------------------------------------------------

std::ostream& operator<<( std::ostream& r_stream, const JSONObject& r_json )
{
    CIE_BEGIN_EXCEPTION_TRACING

    return r_stream << r_json.contents();

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::io
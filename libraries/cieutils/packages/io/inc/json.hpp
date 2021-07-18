#ifndef CIE_UTILS_IO_JSON_HPP
#define CIE_UTILS_IO_JSON_HPP

// --- External Includes ---
#include "nlohmann/json_fwd.hpp"

// --- Utility Includes ---
#include "cieutils/packages/stl_extension/inc/RuntimeConst.hpp" 
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- STL Includes ---
#include <istream>
#include <ostream>
#include <filesystem>


namespace cie::io {
class JSONObject;
} // namespace cie::io


namespace cie::concepts::io {

template <class T>
concept SupportedBase
=  std::same_as<typename std::decay<T>::type, bool>
|| std::same_as<typename std::decay<T>::type, int>
|| std::same_as<typename std::decay<T>::type, Size>
|| std::same_as<typename std::decay<T>::type, float>
|| std::same_as<typename std::decay<T>::type, double>
|| std::same_as<typename std::decay<T>::type, std::string>
|| std::same_as<typename std::decay<T>::type, cie::io::JSONObject>;

template <class T>
concept SupportedType
=  SupportedBase<T>
|| (STLContainer<T> && SupportedBase<typename std::decay<T>::type::value_type>);
} // namespace cie::concepts::detail



namespace cie::io
{


/** Basic interface for a json parser
 *  @details supported types for io:
 *      bool
 *      int
 *      cie::Size
 *      float
 *      double
 *      std::string
 *      JSONObject
 *
 *      std::vector of any above except bool
 *      std::array of any above with size 1, 2, or 3
 */
class JSONObject
{
public:
    using content_type = nlohmann::json;

public:
    JSONObject();

    JSONObject( const JSONObject& r_rhs );

    JSONObject( JSONObject&& r_rhs );

    JSONObject& operator=( const JSONObject& r_rhs );

    JSONObject( const std::string& r_jsonString );

    JSONObject( std::string&& r_jsonString );

    JSONObject( const std::filesystem::path& r_filePath );

    JSONObject( std::istream& r_stream );

    /// Constructor for operator[]
    JSONObject( content_type* p_contents,
                const JSONObject* p_root );

    /// Constructor for operator[] const
    JSONObject( const content_type* p_contents,
                const JSONObject* p_root );

    ~JSONObject();

    /// Get the value associated to the specified key
    JSONObject operator[]( const std::string& r_key );

    /// Get the value associated to the specified key
    const JSONObject operator[]( const std::string& r_key ) const;

    /// Get the specified component if this is an array
    JSONObject operator[]( Size index );

    /// Get the specified component if this is an array
    const JSONObject operator[]( Size index ) const;

    /** Create a new string item with the specified key and value
     *  @details implicitly converts char arrays to std::string
     */
    JSONObject& add( const std::string& r_key,
                     const std::string& r_value,
                     bool allowOverwrite = false );

    /// Create a new item with the specified key and value
    template <concepts::io::SupportedType ValueType>
    JSONObject& add( const std::string& r_key,
                     const ValueType& r_value,
                     bool allowOverwrite = false );

    /** Set the json value to the specified string
     *  @details implicitly converts char arrays to std::string
     */
    JSONObject& set( const std::string& r_value );

    /// Set the json value to the specified one
    template <concepts::io::SupportedType ValueType>
    JSONObject& set( const ValueType& r_value );

    /// Parse this as an instance of the template parameter
    template <concepts::io::SupportedType ValueType>
    ValueType as() const;

    /// Return true if this can be converted to an instance of the template parameter
    template <concepts::io::SupportedType ValueType>
    bool is() const;

    /// Check if this is a json array
    bool isArray() const;

    /// Check if this is a json object
    bool isObject() const;

    /// Return true if this has an item with a matching key
    bool hasKey( const std::string& r_key ) const;

    /// Item size
    Size size() const;

    /// Get wrapped object
    const content_type& contents() const;

    /// Get the root that holds the resources
    const JSONObject& root() const;

private:
    /// Set / get helper class
    template <class ValueType>
    struct SetGet
    {
        static void set( JSONObject& r_json,
                         const ValueType& r_value );

        static ValueType as( const JSONObject& r_json );
    };

    /// Type identification and initialization helper
    template <class ValueType>
    struct TypeQuery
    {
        static bool is( const JSONObject& r_json );

        static JSONObject addDefault( JSONObject& r_json,
                                      const std::string& r_key );
    };

    /// Type-safe initialization of a new item's value
    template <concepts::io::SupportedType ValueType>
    JSONObject addDefault( const std::string& r_key );

protected:
    utils::RuntimeConst<content_type> _p_contents;

private:
    const JSONObject* _p_root;
}; // class JSONObject


std::ostream& operator<<( std::ostream& r_stream, const JSONObject& r_json );


} // namespace cie::io

#include "cieutils/packages/io/impl/json_impl.hpp"

#endif
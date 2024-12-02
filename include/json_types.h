/*
 * Repository:  https://github.com/kingkybel/JsonObject
 * File Name:   include/json_types.h
 * Description: definition of types used for json classes
 *              currently the types are aliases of boost::json::* types, but the underlying json library
 *              may be replaced, For that reason the fundamental types are defined here to make replacement easier.
 *
 * Copyright (C) 2024 Dieter J Kybelksties <github@kybelksties.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * @date: 2024-08-15
 * @author: Dieter J Kybelksties
 */

#ifndef NS_UTIL_JSON_TYPES_H_INCLUDED
#define NS_UTIL_JSON_TYPES_H_INCLUDED

#include <boost/json.hpp>
#include <boost/json/kind.hpp>
#include <string>
#include <string_view>

namespace std
{
inline std::string to_string(boost::json::value const &val)
{
    return val.as_string().data();
}
}

namespace util
{
// Order matters
enum class kind : unsigned char
{
    /// The null value.
    null = static_cast<unsigned char>(boost::json::kind::null),

    /// A `bool`.
    bool_ = static_cast<unsigned char>(boost::json::kind::bool_),

    /// A `std::int64_t`
    int64 = static_cast<unsigned char>(boost::json::kind::int64),

    /// A `std::uint64_t`
    uint64 = static_cast<unsigned char>(boost::json::kind::uint64),

    /// A `double`.
    double_ = static_cast<unsigned char>(boost::json::kind::double_),

    /// A @ref string.
    string = static_cast<unsigned char>(boost::json::kind::string),

    /// An @ref array.
    array = static_cast<unsigned char>(boost::json::kind::array),

    /// An @ref object.
    object = static_cast<unsigned char>(boost::json::kind::object)
};
using key_type    = boost::json::string_view;
using value_type  = boost::json::value;
using object_type = boost::json::object;
using array_type  = boost::json::array;
using json_error  = boost::system::error_code;

inline value_type json_parse(std::string const &jstr)
{
    return boost::json::parse(jstr);
}

inline std::string json_serialize(value_type const &jvalue)
{
    return boost::json::serialize(jvalue);
}

inline void array_prepend(array_type &arr, value_type const &value)
{
    arr.insert(arr.begin(), value);
}

inline void array_append(array_type &arr, value_type const &value)
{
    arr.insert(arr.end(), value);
}

inline void array_resize(array_type &arr, size_t newSize)
{
    arr.resize(newSize);
}

inline size_t array_size(array_type const &arr)
{
    return arr.size();
}

inline value_type from_json_string(std::string const &json_str)
{
    return boost::json::parse(json_str);
}

inline std::string to_json_string(value_type const &json_val)
{
    return boost::json::serialize(json_val);
}

inline bool is_bool(value_type const &val)
{
    return val.is_bool();
}

inline bool is_int64(value_type const &val)
{
    return val.is_int64();
}

inline bool is_uint64(value_type const &val)
{
    return val.is_uint64();
}

inline bool is_double(value_type const &val)
{
    return val.is_double();
}

inline bool is_string(value_type const &val)
{
    return val.is_string();
}

inline bool is_primitive(value_type const &val)
{
    return val.is_primitive();
}

inline bool is_structured(value_type const &val)
{
    return val.is_structured();
}

inline bool is_array(value_type const &val)
{
    return val.is_array();
}

inline bool is_array(value_type const *val)
{
    return val != nullptr && val->is_array();
}

inline bool is_object(value_type const *val)
{
    return val != nullptr && val->is_object();
}

inline bool is_object(value_type const &val)
{
    return val.is_object();
}

inline bool as_bool(value_type &&val)
{
    return val.as_bool();
}

inline std::int64_t as_int64(value_type &&val)
{
    return val.as_int64();
}

inline std::uint64_t as_uint64(value_type &&val)
{
    return val.as_uint64();
}

inline double as_double(value_type &&val)
{
    return val.as_double();
}

inline std::string as_string(value_type &val)
{
    return val.as_string().data();
}

inline array_type &as_array(value_type &val)
{
    return val.as_array();
}

inline array_type const &as_array(value_type const &val)
{
    return val.as_array();
}

inline array_type *as_array(value_type *val)
{
    return &val->as_array();
}

inline array_type const *as_array(value_type const *val)
{
    return &val->as_array();
}

inline object_type &as_object(value_type &val)
{
    return val.as_object();
}

inline object_type const &as_object(value_type const &val)
{
    return val.as_object();
}

inline object_type *as_object(value_type *val)
{
    return &val->as_object();
}

inline object_type const *as_object(value_type const *val)
{
    return &val->as_object();
}

} // namespace util

#endif // NS_UTIL_JSON_TYPES_H_INCLUDED

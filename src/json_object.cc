/*
 * Repository:  https://github.com/kingkybel/JsonObject
 * File Name:   src/json_object.cc
 * Description: json object class
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
 * @date: 2024-11-30
 * @author: Dieter J Kybelksties
 */

#include "json_object.h"

#include <sstream>

namespace util
{
JsonObject::JsonObject()
    : json_(object_type{})
{
}

JsonObject::JsonObject(std::string const& jsonStr)
{
    json_ = boost::json::parse(jsonStr);
}

[[nodiscard]] std::string JsonObject::toString() const
{
    return boost::json::serialize(json_);
}

[[nodiscard]] value_type
    JsonObject::get(std::string const& path, std::optional<value_type> const& defaultValue) const
{
    return get(JsonKeyPath{path}, defaultValue);
}

[[nodiscard]] value_type
    JsonObject::get(JsonKeyPath const& path, std::optional<value_type> const& defaultValue) const
{
    value_type const* current = &json_;
    for (auto const& key: path.getKeys())
    {
        if (key->isIndex())
        {
            auto const* indexKey = dynamic_cast<JsonIndexKey*>(key.get());
            if (!current->is_array())
            {
                throw std::invalid_argument("key '" + key->toString() + "' and array-container are incompatible");
            }
            int64_t idx = indexKey->getIndex(current->as_array());
            if (idx < 0 || idx >= static_cast<int64_t>(current->as_array().size()))
            {
                if (!defaultValue)
                {
                    std::ostringstream ss;
                    ss << "Index '" << idx <<"' is out of bounds [0.." << current->as_array().size() << "]";
                    throw std::invalid_argument(ss.str());
                }
                return defaultValue.value();
            }
            current = &current->as_array()[idx];
        }
        else
        {
            auto const* stringKey = dynamic_cast<JsonStringKey*>(key.get());
            if (!current->is_object())
            {
                throw std::invalid_argument("key '" + key->toString() + "' and array-container are incompatible");
            }
            if (!current->as_object().contains(stringKey->getKey()) && defaultValue)
            {
                return defaultValue.value();
            }
            current = &current->as_object().at(stringKey->getKey());
        }
    }
    return *current;
}

void JsonObject::set(std::string const& path, value_type const& value, bool force)
{
    set(JsonKeyPath{path}, value, force);
}

void JsonObject::set(JsonKeyPath const& path, value_type const& value, bool force)
{
    value_type* current = &json_;
    for (size_t i = 0; i < path.getKeys().size(); ++i)
    {
        auto const& key = path.getKeys()[i];
        if (key->isIndex())
        {
            auto* indexKey = dynamic_cast<JsonIndexKey*>(key.get());
            if (!current->is_array())
            {
                if (force)
                {
                    *current = boost::json::array();
                }
                else
                {
                    throw std::runtime_error("Expected array at key: " + path.toString());
                }
            }
            auto&   arr = current->as_array();
            int64_t idx = indexKey->getIndex(arr);
            if (idx == static_cast<int64_t>(arr.size()) || idx < 0)
            {
                if (force)
                {
                    arr.resize(idx + 1);
                }
                else
                {
                    throw std::runtime_error("Index out of range: " + path.toString());
                }
            }
            if (i == path.getKeys().size() - 1)
            {
                arr[idx] = value;
                return;
            }
            current = &arr[idx];
        }
        else
        {
            auto* stringKey = dynamic_cast<JsonStringKey*>(key.get());
            if (!current->is_object())
            {
                if (force)
                {
                    *current = object_type();
                }
                else
                {
                    throw std::runtime_error("Expected object at key: " + path.toString());
                }
            }
            auto& obj = current->as_object();
            if (i == path.getKeys().size() - 1)
            {
                obj[stringKey->getKey()] = value;
                return;
            }
            if (!obj.contains(stringKey->getKey()))
            {
                if (force)
                {
                    obj[stringKey->getKey()] = object_type();
                }
                else
                {
                    throw std::runtime_error("Missing key: " + stringKey->getKey());
                }
            }
            current = &obj[stringKey->getKey()];
        }
    }
}

std::ostream&
    JsonObject::prettyPrint(std::ostream& os, value_type const& jv, int indentWidth, std::string* indent) const
{
    std::string indent_;
    if(!indent)
        indent = &indent_;
    switch(static_cast<kind>(jv.kind()))
    {
        case kind::object:
        {
            os << "{\n";
            indent->append(indentWidth, ' ');
            auto const& obj = jv.get_object();
            if(!obj.empty())
            {
                auto it = obj.begin();
                for(;;)
                {
                    os << *indent << to_json_string(it->key()) << " : ";
                    prettyPrint(os, it->value(), indentWidth, indent);
                    if(++it == obj.end())
                        break;
                    os << ",\n";
                }
            }
            os << "\n";
            indent->resize(indent->size() - indentWidth);
            os << *indent << "}";
            break;
        }

        case kind::array:
        {
            os << "[\n";
            indent->append(indentWidth, ' ');
            auto const& arr = jv.get_array();
            if(!arr.empty())
            {
                auto it = arr.begin();
                for(;;)
                {
                    os << *indent;
                    prettyPrint(os, *it, indentWidth, indent);
                    if(++it == arr.end())
                        break;
                    os << ",\n";
                }
            }
            os << "\n";
            indent->resize(indent->size() - indentWidth);
            os << *indent << "]";
            break;
        }

        case kind::string:
        {
            os << to_json_string(jv.get_string());
            break;
        }

        case kind::uint64:
            os << jv.get_uint64();
            break;

        case kind::int64:
            os << jv.get_int64();
            break;

        case kind::double_:
            os << jv.get_double();
            break;

        case kind::bool_:
            if(jv.get_bool())
                os << "true";
            else
                os << "false";
            break;

        case kind::null:
            os << "null";
            break;
    }

    if(indent->empty())
        os << "\n";

    return os;
}

} // namespace util

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

namespace util
{
JsonObject::JsonObject()
    : json_(boost::json::object{})
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

[[nodiscard]] boost::json::value JsonObject::get(std::string const& path, boost::json::value const& defaultValue) const
{
    return get(JsonKeyPath{path}, defaultValue);
}

[[nodiscard]] boost::json::value JsonObject::get(JsonKeyPath const& path, boost::json::value const& defaultValue) const
{
    boost::json::value const* current = &json_;
    for (auto const& key: path.getKeys())
    {
        if (key->isIndex())
        {
            auto const* indexKey = dynamic_cast<JsonIndexKey*>(key.get());
            if (!current->is_array())
            {
                return defaultValue;
            }
            int64_t idx = indexKey->getIndex(current->as_array());
            if (idx < 0 || idx >= static_cast<int64_t>(current->as_array().size()))
            {
                return defaultValue;
            }
            current = &current->as_array()[idx];
        }
        else
        {
            auto const* stringKey = dynamic_cast<JsonStringKey*>(key.get());
            if (!current->is_object() || !current->as_object().contains(stringKey->getKey()))
            {
                return defaultValue;
            }
            current = &current->as_object().at(stringKey->getKey());
        }
    }
    return *current;
}

void JsonObject::set(std::string const& path, boost::json::value const& value, bool force)
{
    set(JsonKeyPath{path}, value, force);
}

void JsonObject::set(JsonKeyPath const& path, boost::json::value const& value, bool force)
{
    boost::json::value* current = &json_;
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
                    *current = boost::json::object();
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
                    obj[stringKey->getKey()] = boost::json::object();
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
} // namespace util

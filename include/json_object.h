/*
 * Repository:  https://github.com/kingkybel/JsonObject
 * File Name:   include/json_object.h
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

#ifndef NS_UTIL_JSON_OBJECT_H_INCLUDED
#define NS_UTIL_JSON_OBJECT_H_INCLUDED

#include "json_key_path.h"
#include "json_types.h"

namespace util
{

/**
 * A class to handle json objects.
 * Keys in the object are addressed by paths constructed from index-keys and string-keys_.
 * Index-keys can be any positive number enclosed in square brackets or [^] or [$]; e.g [^], [$], [0], [123]
 * String keys can be any string not containing '[', ']', '\\n', '\\r', only numbers, empty or whitespace-only strings
 * or strings that start or end in whitespace.
 *
 * <br><br>For example valid paths:
 * <ul>
 * <li>"key"</li>
 * <li>"[0]"</li>
 * <li>"key/[2]/key3/[^]"</li>
 * <li>"a/b/c/d/e/f"</li>
 * <li>"[^]/[^]/[^]/[^]/[^]/key"</li>
 * </ul>
 *
 * <br><br>For example invalid paths:
 * <ul>
 * <li>"[key]"</li>
 * <li>"[0] "</li>
 * <li>"key/[2]/key]3/[^]"</li>
 * <li>"a/b/666/d/e/f"</li>
 * <li>"[^]/[^ ]/[ ]/[]/[^]/key"</li>
 * </ul>
 */
class JsonObject
{
    value_type json_{};

  public:
    JsonObject();
    explicit JsonObject(std::string const& jsonStr);

    [[nodiscard]] value_type
        get(std::string const& path, std::optional<value_type> const& defaultValue = std::optional<value_type>{}) const;

    [[nodiscard]] value_type
        get(JsonKeyPath const& path, std::optional<value_type> const& defaultValue = std::optional<value_type>{}) const;

    void set(std::string const& path, value_type const& value, bool force = false);
    void set(JsonKeyPath const& path, value_type const& value, bool force = false);

    [[nodiscard]] std::string toString(int indent = 4) const;
    std::ostream&
        prettyPrint(std::ostream& os, value_type const& jv, int indentWidth = 4, std::string* indent = nullptr) const;
};

} // namespace util

#endif // NS_UTIL_JSON_OBJECT_H_INCLUDED

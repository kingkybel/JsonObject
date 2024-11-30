/*
 * Repository:  https://github.com/kingkybel/JsonObject
 * File Name:   include/json_key_path.h
 * Description: class for json key paths
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

#ifndef NS_UTIL_JSON_KEY_PATH_H_INCLUDED
#define NS_UTIL_JSON_KEY_PATH_H_INCLUDED

#include <boost/json.hpp>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

namespace util
{
/**
 * Abstract base class for json key.
 */
class JsonKey
{
  public:
    virtual ~JsonKey()                                 = default;
    [[nodiscard]] virtual std::string toString() const = 0;
    [[nodiscard]] virtual bool        isIndex() const  = 0;
};

/**
 * Json string key implementation for dictionary keys.
 */
class JsonStringKey : public JsonKey
{
    std::string key_;

  public:
    explicit JsonStringKey(std::string const &key);
    [[nodiscard]] std::string        toString() const override;
    [[nodiscard]] bool               isIndex() const override;
    [[nodiscard]] std::string const &getKey() const;
};

/**
 * Json index key implementation for list indices.
 */
class JsonIndexKey : public JsonKey
{
    int64_t index_{};
    bool    isStartSymbol_ = false;
    bool    isEndSymbol_   = false;

  public:
    explicit JsonIndexKey(std::string const &idx);
    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] bool        isIndex() const override;
    [[nodiscard]] int64_t     getIndex(boost::json::array const &array) const;
};

/**
 * Json key path class.
 */
class JsonKeyPath
{
    std::vector<std::shared_ptr<JsonKey>> keys;

  public:
    explicit JsonKeyPath(std::string const &path);
    [[nodiscard]] std::vector<std::shared_ptr<JsonKey>> const &getKeys() const;
    [[nodiscard]] std::string                                  toString() const;
};

} // namespace util

#endif // NS_UTIL_JSON_KEY_PATH_H_INCLUDED

/*
 * Repository:  https://github.com/kingkybel/JsonObject
 * File Name:   src/json_key_path.cc
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
#include "json_key_path.h"

namespace util
{
JsonStringKey::JsonStringKey(std::string const &key)
    : key_(key)
{
    if (key_.empty())
    {
        throw std::invalid_argument("JsonStringKey cannot be empty string");
    }
    if (key[0] == ' ' || key[0] == '\t' || key[key.size() - 1] == ' ' || key[key.size() - 1] == '\t')
    {
        throw std::invalid_argument("JsonStringKey cannot start or end in whitespace");
    }
    if (key_.contains('[') || key_.contains(']') || key_.contains('\n') || key_.contains('\r'))
    {
        throw std::invalid_argument("JsonStringKey cannot contain `[`,`]`, `\\n` or `\\r`");
    }
    if (key_.contains('[') || key.contains(']') || key_.contains('\n') || key_.contains('\r'))
    {
        throw std::invalid_argument("JsonStringKey cannot contain `[`,`]`, `\\n` or `\\r`");
    }
    if (key_.find_first_not_of("0123456789") == std::string::npos)
    {
        throw std::invalid_argument("JsonStringKey must contain at least one non-numeric character");
    }
}

std::string JsonStringKey::toString() const
{
    return key_;
}

bool JsonStringKey::isIndex() const
{
    return false;
}

std::string const &JsonStringKey::getKey() const
{
    return key_;
}

JsonIndexKey::JsonIndexKey(std::string const &idx)
{
    if (idx.size() < 3 || idx[0] != '[' || idx[idx.size() - 1] != ']' ||
        idx.find_first_not_of("[0123456789]^$") != std::string::npos)
    {
        throw std::invalid_argument("JsonIndexKeys must be of format '\\[^|$|[0-9]+\\]'");
    }
    if (idx == "[^]")
    {
        isStartSymbol_ = true;
        index_         = 0; // Start index_
    }
    else if (idx == "[$]")
    {
        isEndSymbol_ = true;
        index_       = -1; // Placeholder for end index_
    }
    else
    {
        try
        {
            auto withoutBrackets = idx.substr(1, idx.size() - 2);
            index_               = std::stoi(withoutBrackets);
        }
        catch (...)
        {
            throw std::invalid_argument("Cannot create valid index_ from '" + idx + "'");
        }
        if (index_ < 0)
        {
            throw std::invalid_argument("Index cannot be negative: " + idx);
        }
    }
}

std::string JsonIndexKey::toString() const
{
    if (isStartSymbol_)
    {
        return "[^]";
    }
    if (isEndSymbol_)
    {
        return "[$]";
    }
    return "[" + std::to_string(index_) + "]";
}

bool JsonIndexKey::isIndex() const
{
    return true;
}

bool JsonIndexKey::isStartSymbol() const
{
    return isStartSymbol_;
}

bool JsonIndexKey::isEndSymbol() const
{
    return isEndSymbol_;
}

int64_t JsonIndexKey::getIndex(boost::json::array const &array) const
{
    if (isStartSymbol_)
    {
        return 0;
    }
    if (isEndSymbol_)
    {
        return static_cast<int64_t>(array.size() - 1);
    }
    return index_;
}

JsonKeyPath::JsonKeyPath(std::string const &path)
{
    std::istringstream stream(path);
    std::string        segment;
    if (path.empty())
    {
        throw std::invalid_argument("Empty JsonKeyPath is not allowed");
    }
    while (std::getline(stream, segment, '/'))
    {
        if (!segment.empty())
        {
            if (segment.front() == '[' && segment.back() == ']')
            {
                keys_.emplace_back(std::make_shared<JsonIndexKey>(segment));
            }
            else
            {
                keys_.emplace_back(std::make_shared<JsonStringKey>(segment));
            }
        }
    }
}

std::vector<std::shared_ptr<JsonKey>> const &JsonKeyPath::getKeys() const
{
    return keys_;
}

std::string JsonKeyPath::toString() const
{
    std::ostringstream oss;
    for (size_t i = 0UL; i < keys_.size(); ++i)
    {
        oss << keys_[i]->toString() << (i < keys_.size() - 1 ? "/" : "");
    }
    return oss.str();
}

} // namespace util

/*
 * Repository:  https://github.com/kingkybel/JsonObject
 * File Name:   test/json_key_path_tests.cc
 * Description: Unit tests json key_ paths
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

#include <gtest/gtest.h>
#include <string>

using namespace std;
using namespace util;

class JsonKeyPathTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        // just in case
    }

    void TearDown() override
    {
        // just in case
    }
};

TEST_F(JsonKeyPathTest, valid_keys_test)
{
    ASSERT_NO_THROW(JsonStringKey("key_"));
    ASSERT_NO_THROW(JsonStringKey("key_ and a bit"));
    ASSERT_NO_THROW(JsonIndexKey("[0]"));
    ASSERT_NO_THROW(JsonIndexKey("[5]"));
    ASSERT_NO_THROW(JsonIndexKey("[^]"));
    ASSERT_NO_THROW(JsonIndexKey("[$]"));
    ASSERT_NO_THROW(JsonStringKey("0_"));
    ASSERT_NO_THROW(JsonStringKey("_1"));
    ASSERT_NO_THROW(JsonStringKey("3.1415"));
}

TEST_F(JsonKeyPathTest, invalid_keys_test)
{
    ASSERT_THROW(JsonStringKey(""), std::invalid_argument);
    ASSERT_THROW(JsonStringKey(" "), std::invalid_argument);
    ASSERT_THROW(JsonStringKey("\t"), std::invalid_argument);
    ASSERT_THROW(JsonStringKey("[key_"), std::invalid_argument);
    ASSERT_THROW(JsonStringKey("key_ and ]a bit"), std::invalid_argument);
    ASSERT_THROW(JsonStringKey("[0]"), std::invalid_argument);
    ASSERT_THROW(JsonStringKey("[5]"), std::invalid_argument);
    ASSERT_THROW(JsonStringKey("[$]"), std::invalid_argument);
    ASSERT_THROW(JsonStringKey("[^]"), std::invalid_argument);

    ASSERT_THROW(JsonIndexKey(" [0]"), std::invalid_argument);
    ASSERT_THROW(JsonIndexKey("[5] "), std::invalid_argument);
    ASSERT_THROW(JsonIndexKey("[ 4711 ]"), std::invalid_argument);
    ASSERT_THROW(JsonIndexKey("^]"), std::invalid_argument);
    ASSERT_THROW(JsonIndexKey("[$"), std::invalid_argument);
    ASSERT_THROW(JsonIndexKey("$"), std::invalid_argument);
    ASSERT_THROW(JsonIndexKey("^"), std::invalid_argument);
    ASSERT_THROW(JsonIndexKey("key "), std::invalid_argument);
    ASSERT_THROW(JsonIndexKey("\tkey "), std::invalid_argument);
    ASSERT_THROW(JsonIndexKey("0"), std::invalid_argument);
    ASSERT_THROW(JsonIndexKey("123"), std::invalid_argument);
}

TEST_F(JsonKeyPathTest, valid_key_paths_test)
{
    ASSERT_NO_THROW(JsonKeyPath("key_"));
    ASSERT_NO_THROW(JsonKeyPath("key_ and a bit/[0]"));
    ASSERT_NO_THROW(JsonKeyPath("[0]/key_/[$]/[^]"));
    ASSERT_NO_THROW(JsonKeyPath("[^]"));
    ASSERT_NO_THROW(JsonKeyPath("[$]"));
}

TEST_F(JsonKeyPathTest, invalid_key_paths_test)
{
    ASSERT_THROW(JsonKeyPath("1/2/3/4"), std::invalid_argument);
    ASSERT_THROW(JsonKeyPath("a/b/[ 4]"), std::invalid_argument);
    ASSERT_THROW(JsonKeyPath("a/b/[4 ]"), std::invalid_argument);
}

TEST_F(JsonKeyPathTest, key_path_accessors_and_roundtrip_test)
{
    JsonKeyPath keyPath("a/[0]/b/[$]");
    ASSERT_EQ(keyPath.size(), 4UL);
    ASSERT_EQ(keyPath.toString(), "a/[0]/b/[$]");
    ASSERT_FALSE(keyPath.getKeys()[0]->isIndex());
    ASSERT_TRUE(keyPath.getKeys()[1]->isIndex());
}

TEST_F(JsonKeyPathTest, empty_and_slashy_paths_test)
{
    ASSERT_THROW(JsonKeyPath(""), std::invalid_argument);

    JsonKeyPath normalized("/a//b/[0]///");
    ASSERT_EQ(normalized.size(), 3UL);
    ASSERT_EQ(normalized.toString(), "a/b/[0]");
}

TEST_F(JsonKeyPathTest, index_key_get_index_test)
{
    boost::json::array arr;
    arr.push_back(10);
    arr.push_back(20);

    JsonIndexKey first("[^]");
    JsonIndexKey last("[$]");
    JsonIndexKey middle("[1]");

    ASSERT_EQ(first.getIndex(arr), 0);
    ASSERT_EQ(last.getIndex(arr), 1);
    ASSERT_EQ(middle.getIndex(arr), 1);
}

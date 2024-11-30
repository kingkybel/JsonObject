/*
 * Repository:  https://github.com/kingkybel/JsonObject
 * File Name:   test/json_object_tests.cc
 * Description: Unit tests json object
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

#include <gtest/gtest.h>
#include <string>

using namespace std;
using namespace util;

class JsonObjectTest : public ::testing::Test
{
    protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(JsonObjectTest, simple_set_and_get_key_value_tests)
{
    auto jsonObj = JsonObject{R"(
                                {"key":"value"}
                                )"
    };
    ASSERT_NO_THROW(jsonObj.set("key","value2"));
    ASSERT_EQ(jsonObj.get("key"), "value2");

    jsonObj = JsonObject{R"(
    [1,2,3,4,5]
    )"};
    ASSERT_NO_THROW(jsonObj.set("[2]",-666));
    ASSERT_EQ(jsonObj.get("[2]"), -666);

    jsonObj = JsonObject{R"(
                            [
                                {},
                                {"key":"value"},
                                ["a","b","c"]
                            ]
                            )"
    };
    ASSERT_NO_THROW(jsonObj.set("[0]/key0","value0"));
    ASSERT_EQ(jsonObj.get("[0]/key0"), "value0");

    ASSERT_NO_THROW(jsonObj.set("[1]/key","new value"));
    ASSERT_EQ(jsonObj.get("[1]/key"), "new value");

    ASSERT_NO_THROW(jsonObj.set("[2]/[1]","BBB"));
    ASSERT_EQ(jsonObj.get("[2]/[1]"), "BBB");
}

TEST_F(JsonObjectTest, set_and_get_default_key_value_tests)
{
    auto jsonObj = JsonObject{R"(
                                []
                                )"
    };
    ASSERT_EQ(jsonObj.get("[0]","defaultValue"), "defaultValue");
    ASSERT_EQ(jsonObj.get("[0]/key0","defaultValue"), "defaultValue");

    jsonObj = JsonObject{R"(
                                {}
                                )"
    };
    ASSERT_EQ(jsonObj.get("key","defaultValue"), "defaultValue");
    ASSERT_EQ(jsonObj.get("key/[0]/key1","defaultValue"), "defaultValue");
}
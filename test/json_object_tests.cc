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

#include <cstdio>
#include <fstream>
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

TEST_F(JsonObjectTest, simple_set_and_get_tests)
{
    auto jsonObj = JsonObject{R"(
                                {"key":"value"}
                                )"};
    ASSERT_NO_THROW(jsonObj.set("key", "value2"));
    ASSERT_EQ(jsonObj.get("key"), "value2");

    jsonObj = JsonObject{R"(
    [1,2,3,4,5]
    )"};
    ASSERT_NO_THROW(jsonObj.set("[2]", -666));
    ASSERT_EQ(jsonObj.get("[2]"), -666);

    jsonObj = JsonObject{R"(
                            [
                                {},
                                {"key":"value"},
                                ["a","b","c"]
                            ]
                            )"};
    ASSERT_NO_THROW(jsonObj.set("[0]/key0", "value0"));
    ASSERT_EQ(jsonObj.get("[0]/key0"), "value0");

    ASSERT_NO_THROW(jsonObj.set("[1]/key", "new value"));
    ASSERT_EQ(jsonObj.get("[1]/key"), "new value");

    ASSERT_NO_THROW(jsonObj.set("[2]/[1]", "BBB"));
    ASSERT_EQ(jsonObj.get("[2]/[1]"), "BBB");
}

TEST_F(JsonObjectTest, set_and_get_default_tests)
{
    auto jsonObj = JsonObject{R"(
                                []
                                )"};
    ASSERT_EQ(jsonObj.get("[0]", "defaultValue"), "defaultValue");
    ASSERT_EQ(jsonObj.get("[0]/key0", "defaultValue"), "defaultValue");

    jsonObj = JsonObject{R"(
                                {}
                                )"};
    ASSERT_EQ(jsonObj.get("key", "defaultValue"), "defaultValue");
    ASSERT_EQ(jsonObj.get("key/[0]/key1", "defaultValue"), "defaultValue");
}

TEST_F(JsonObjectTest, set_and_get_failing_default_tests)
{
    auto jsonObj = JsonObject{R"(
                                []
                                )"};
    // NOLINTNEXTLINE
    ASSERT_THROW(auto x = jsonObj.get("key", "defaultValue"), std::invalid_argument);

    jsonObj = JsonObject{R"(
                                {}
                                )"};

    // NOLINTNEXTLINE
    ASSERT_THROW(auto x = jsonObj.get("[0]", "defaultValue"), std::invalid_argument);

    jsonObj = JsonObject{R"(
                                [{"key":"value"}]
                                )"};
    // NOLINTNEXTLINE
    ASSERT_THROW(auto x = jsonObj.get("[0]/[1]", "defaultValue"), std::invalid_argument);

    jsonObj = JsonObject{R"(
                                {"key":["value"]}
                                )"};
    // NOLINTNEXTLINE
    ASSERT_THROW(auto x = jsonObj.get("key/key2", "defaultValue"), std::invalid_argument);
}

TEST_F(JsonObjectTest, get_special_symbol_tests)
{
    auto       jsonObj = JsonObject{R"(
                                [
                                    [1,2,3],
                                    ["a","b","c"]
                                ]
                                )"};
    value_type v;
    ASSERT_NO_THROW(v = jsonObj.get("[^]"));
    ASSERT_EQ(as_array(v)[0], 1);

    ASSERT_NO_THROW(v = jsonObj.get("[$]"));
    ASSERT_EQ(as_array(v)[2], "c");

    ASSERT_NO_THROW(v = jsonObj.get("[^]/[$]"));
    ASSERT_EQ(v, 3);

    ASSERT_NO_THROW(v = jsonObj.get("[$]/[^]"));
    ASSERT_EQ(v, "a");

    jsonObj = JsonObject{R"(
                                [
                                    [],
                                    ["a","b","c"]
                                ]
                                )"};
    ASSERT_THROW(v = jsonObj.get("[^]/[0]"), std::invalid_argument);
    ASSERT_THROW(v = jsonObj.get("[0]/[$]"), std::invalid_argument);
}

TEST_F(JsonObjectTest, set_special_symbol_no_force_tests)
{
    auto jsonObj = JsonObject{R"(
                                []
                                )"};
    ASSERT_NO_THROW(jsonObj.set("[^]", 1));
    ASSERT_EQ(jsonObj.get("[0]"), 1);
    ASSERT_NO_THROW(jsonObj.set("[$]", 2));
    ASSERT_EQ(jsonObj.get("[0]"), 1);
    ASSERT_EQ(jsonObj.get("[1]"), 2);

    jsonObj = JsonObject{R"(
                                [[]]
                                )"};
    ASSERT_THROW(jsonObj.set("[^]/[^]", 1), std::invalid_argument);
    ASSERT_NO_THROW(jsonObj.set("[0]/[^]", 1));
    ASSERT_EQ(jsonObj.get("[0]/[0]"), 1);
}

TEST_F(JsonObjectTest, set_special_symbol_force_tests)
{
    auto jsonObj = JsonObject{R"(
                                [
                                    {},
                                    {
                                        "k1":"v1",
                                        "k2":[]
                                    }
                                ]
                                )"};
    ASSERT_NO_THROW(jsonObj.set("[^]", "insertedAtPosZero", true));
    ASSERT_EQ(jsonObj.get("[0]"), "insertedAtPosZero");
    ASSERT_NO_THROW(jsonObj.set("[$]", "insertedAtEnd", true));
    ASSERT_EQ(jsonObj.get("[$]"), "insertedAtEnd");
    ASSERT_NO_THROW(jsonObj.set("[^]/[$]/[^]", "insertedNewPosZero", true));
    ASSERT_EQ(jsonObj.get("[0]/[0]/[0]"), "insertedNewPosZero");
}

TEST_F(JsonObjectTest, load_write_tests)
{
    auto        jsonObj  = JsonObject{R"(
                                [
                                    {},
                                    {
                                        "k1":"v1",
                                        "k2":[]
                                    }
                                ]
                                )"};
    std::string filename = "./JsonObjectTest_load_write_tests.json";
    jsonObj.write(filename, 0);
    std::ifstream ifs(filename.c_str());
    std::string   line{};
    std::string   jsonStr{};
    while (getline(ifs, line) && !line.empty())
    {
        jsonStr += line;
    }
    ASSERT_EQ(jsonStr, R"([{},{"k1":"v1","k2":[]}])");
    ifs.close();

    jsonObj.clear();
    ASSERT_THROW(auto x = jsonObj.get("[0]"), std::invalid_argument);

    jsonObj.load(filename);
    ASSERT_EQ(jsonObj.get("[1]/k1"), "v1");
    std::remove(filename.c_str());
}

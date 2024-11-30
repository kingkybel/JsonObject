#include <iostream>
#include "json_key_path.h"
#include "json_object.h"
using namespace util;

int main() {
    JsonObject obj;

    // Setting values
    obj.set(JsonKeyPath("key1/key2/[^]/key3"), "start_value", true);
    obj.set(JsonKeyPath("key1/key2/[$]/key3"), "end_value", true);

    // Retrieving values
    auto startVal = obj.get(JsonKeyPath("key1/key2/[^]/key3"), "default");
    auto endVal = obj.get(JsonKeyPath("key1/key2/[$]/key3"), "default");

    std::cout << "Start Value: " << startVal.as_string() << std::endl;
    std::cout << "End Value: " << endVal.as_string() << std::endl;

    // Full JSON structure
    std::cout << obj.toString() << std::endl;

    return 0;
}
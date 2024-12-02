add_library(dkjsonobject STATIC json_object.cc json_key_path.cc)
target_link_libraries(dkjsonobject PRIVATE boost_json)

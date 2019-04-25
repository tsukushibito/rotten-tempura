cmake_minimum_required(VERSION 3.12)

add_executable(temp_base_test main.cpp)

target_include_directories(temp_base_test PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(
    temp_base_test
    temp_base
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

add_test(
    NAME base_test
    COMMAND $<TARGET_FILE:temp_base_test>
)

set_property(
    TEST base_test
    PROPERTY LABELS base base_test
)
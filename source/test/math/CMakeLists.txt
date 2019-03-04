cmake_minimum_required(VERSION 3.12)

add_executable(temp_math_test main.cpp)

target_include_directories(temp_math_test PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(
    temp_math_test
    temp_core
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

add_test(
    NAME math_test
    COMMAND $<TARGET_FILE:temp_math_test>
)

set_property(
    TEST math_test
    PROPERTY LABELS math math_test
)
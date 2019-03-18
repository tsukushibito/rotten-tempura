cmake_minimum_required(VERSION 3.12)

add_executable(temp_app_test main.cpp)

target_include_directories(temp_app_test PRIVATE ${Boost_INCLUDE_DIR})

target_link_libraries(
    temp_app_test
    temp_core
    temp_app
    temp_gfx
)

if(Boost_FOUND)
    target_link_libraries(temp_app_test ${Boost_LIBRARIES})
endif(Boost_FOUND)

source_group("temp_app_test" FILES "main.cpp")
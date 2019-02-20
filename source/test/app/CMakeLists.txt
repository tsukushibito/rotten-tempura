cmake_minimum_required(VERSION 3.12)

add_executable(temp_app_test main.cpp)

target_link_libraries(
    temp_app_test
    temp_core
    temp_app
    temp_gfx
)

source_group("temp_app_test" FILES "main.cpp")
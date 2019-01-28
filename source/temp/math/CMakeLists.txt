cmake_minimum_required(VERSION 3.12)

file(GLOB_RECURSE cpp RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB_RECURSE h RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)
file(GLOB_RECURSE mm RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.mm)

if(APPLE)
    set(source_list ${c} ${cpp} ${h} ${hpp} ${mm})
elseif(WIN32)
    set(source_list ${c} ${cpp} ${h} ${hpp})
endif(APPLE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(temp_math STATIC ${source_list})
target_link_libraries(temp_math ${EXTRA_LIBS})

add_source_group(${source_list})
﻿cmake_minimum_required(VERSION 3.12)

file(GLOB_RECURSE cpp RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB_RECURSE h RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)
file(GLOB_RECURSE mm RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.mm)

if(APPLE)
    set(source_list ${c} ${cpp} ${h} ${hpp} ${mm})
    find_library( COCOA_LIBRARY Cocoa )
    set(EXTRA_LIBS ${EXTRA_LIBS} ${COCOA_LIBRARY})
    find_library( OPENGL_LIBRARY OpenGL )
    set(EXTRA_LIBS ${EXTRA_LIBS} ${OPENGL_LIBRARY})
    find_library( IOKIT_LIBRARY IOKit )
    set(EXTRA_LIBS ${EXTRA_LIBS} ${IOKIT_LIBRARY})
    find_library( COREVIDEO_LIBRARY CoreVideo )
    set(EXTRA_LIBS ${EXTRA_LIBS} ${COREVIDEO_LIBRARY})
    find_library( QUARTZCORE_LIBRARY QuartzCore )
    set(EXTRA_LIBS ${EXTRA_LIBS} ${QUARTZCORE_LIBRARY})
    find_library(VULKAN NAMES vulkan PATHS $ENV{VULKAN_SDK}/Frameworks)
    set(EXTRA_LIBS ${EXTRA_LIBS} ${VULKAN})
    add_definitions("-DVK_USE_PLATFORM_MACOS_MVK")
elseif(WIN32)
    set(source_list ${c} ${cpp} ${h} ${hpp})
    find_package(Vulkan REQUIRED)
    include_directories(temp_test PRIVATE Vulkan::Vulkan)
    set(EXTRA_LIBS ${EXTRA_LIBS} Vulkan::Vulkan)
    add_definitions("-DVK_USE_PLATFORM_WIN32_KHR")
    set(EXTRA_LIBS ${EXTRA_LIBS} "OpenGL32.lib")
endif(APPLE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(temp_base STATIC ${source_list})
target_link_libraries(temp_base ${EXTRA_LIBS})

if(TEMP_USE_BOOST)
    if(Boost_FOUND)
        target_include_directories(temp_base PRIVATE ${Boost_INCLUDE_DIRS})
        message("${Boost_INCLUDE_DIRS}")
        target_link_libraries(temp_base ${Boost_LIBRARIES})
        add_definitions("-DTEMP_USE_BOOST")
    endif(Boost_FOUND)
endif(TEMP_USE_BOOST)

add_source_group(${source_list})
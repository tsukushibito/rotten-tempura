cmake_minimum_required(VERSION 3.12)
enable_testing()

project(tempura CXX)

function(add_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE ${_source})
            file(RELATIVE_PATH _source_rel ${CMAKE_CURRENT_SOURCE_DIR} ${_source})
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path ${_source_rel} DIRECTORY)
        string(REPLACE "/" "\\" _group "${_source_path}")
        source_group("${_group}" FILES "${_source}")
    endforeach()
endfunction(add_source_group)

include_directories(./)

if(APPLE)
    add_definitions("-Wall -std=c++17")
elseif(WIN32)
    add_definitions("-W4")
endif(APPLE)

# set(TEMP_USE_BOOST ON CACHE BOOL "To use filesystem in a environment before c++17.")

# if(TEMP_USE_BOOST)
    message("BOOST_ROOT: ${BOOST_ROOT}")
    message("BOOST_INCLUDEDIR: ${BOOST_INCLUDEDIR}")
    message("BOOST_LIBRARYDIR: ${BOOST_LIBRARYDIR}")
    set (Boost_DETAILED_FAILURE_MSG ON)
    find_package(Boost COMPONENTS system filesystem unit_test_framework) 
    if(Boost_FOUND)
        link_directories(${Boost_LIBRARY_DIRS})
    endif(Boost_FOUND)
# endif(TEMP_USE_BOOST)


add_subdirectory(temp)
add_subdirectory(test)
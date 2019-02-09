cmake_minimum_required(VERSION 3.12)

if(TEMP_USE_BOOST)
    find_package(Boost COMPONENTS system filesystem unit_test_framework) 
    if(Boost_FOUND)
    endif(Boost_FOUND)
endif(TEMP_USE_BOOST)

add_subdirectory(core)
add_subdirectory(math)
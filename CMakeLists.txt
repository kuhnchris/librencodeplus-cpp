cmake_minimum_required(VERSION 3.10)

# set the project name
project(rencodeplus)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(rencodeplus librencodeplus.cpp utils.cpp)

set_target_properties(rencodeplus PROPERTIES
                        POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS}
                        )

# install rules
set(installable_libs rencodeplus)
install(TARGETS ${installable_libs} DESTINATION lib)
install(FILES librencodeplus.hpp DESTINATION include)
install(FILES librencodeplus-constants.hpp DESTINATION include)

cmake_minimum_required(VERSION 3.10)

# set the project name
project(rencodeplus)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(DEBUG_OUTPUT "Build with debug outputs" OFF)
option(BUILD_TESTS "Build test apps" OFF)
option(BUILD_TOOLS "Build tools" ON)

set(CMAKE_BUILD_TYPE RelWithDebInfo)
# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(rencodeplus librencodeplus-encode.cpp librencodeplus-utils.cpp librencodeplus-decode.cpp)

set_target_properties(rencodeplus PROPERTIES
                        POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS}
                        )

if(DEBUG_OUTPUT)
  add_definitions(-DDEBUG)
endif()

if (BUILD_TOOLS)
add_executable(decode_file tools/decode-file.cpp)
set_target_properties(decode_file PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/tools)
target_link_libraries(decode_file rencodeplus)
endif()

if (BUILD_TESTS)
add_executable(tests_encode tests/tests-encode.cpp)
set_target_properties(tests_encode PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/tests)
add_executable(tests_decode tests/tests-decode.cpp)
set_target_properties(tests_decode PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/tests)

target_link_libraries(tests_encode rencodeplus)
target_link_libraries(tests_decode rencodeplus)
configure_file(tests/encode-list.bin ${PROJECT_BINARY_DIR}/tests/encode-list.bin)
configure_file(tests/encode-map.bin ${PROJECT_BINARY_DIR}/tests/encode-map.bin)
configure_file(tests/encode-str.bin ${PROJECT_BINARY_DIR}/tests/encode-str.bin)

enable_testing()
add_test(encode tests/tests_encode)
add_test(decode tests/tests_decode)
endif()

# install rules
set(installable_libs rencodeplus)
install(TARGETS ${installable_libs} DESTINATION lib)
install(FILES librencodeplus.hpp DESTINATION include)

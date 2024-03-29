cmake_minimum_required(VERSION 3.4.3)
project(AddLang)

# Define these manually
set(LLVM_PROJECT_DIR "/home/joachim/repos/llvm-project/")
set(LLVM_BUILD_DIR ${LLVM_PROJECT_DIR}/build)

set (CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/)
#set(CMAKE_BUILD_TYPE Debug)

# add LLVM
set(LLVM_DIR ${LLVM_BUILD_DIR}/lib/cmake/llvm)
find_package(LLVM REQUIRED CONFIG)
if( NOT LLVM_FOUND )
  message(FATAL_ERROR "LLVM package can't be found. Set CMAKE_PREFIX_PATH variable to LLVM's installation prefix.")
else()
  message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
  message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
endif()
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Find the libraries that correspond to the LLVM components that we wish to use
llvm_map_components_to_libnames(LLVM_LIBRARIES mcjit executionengine native support core)

# Setup flex and bison
find_package(BISON)
find_package(FLEX)
BISON_TARGET(MyParser frontend/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
FLEX_TARGET(MyScanner frontend/lex.l ${CMAKE_CURRENT_BINARY_DIR}/tokens.cpp)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/frontend)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cxxopts/include)

# Create and link our executable
add_executable(addlang ${BISON_MyParser_OUTPUTS} ${FLEX_MyScanner_OUTPUTS} frontend/node.cpp frontend/ssagenerator.cpp main.cpp)
#target_compile_options(addlang PRIVATE -Wall -Wextra -pedantic -Werror)
target_link_libraries(addlang ${LLVM_LIBRARIES})

# Google test
include_directories(${LLVM_PROJECT_DIR}/llvm/utils/unittest/googletest/include)
enable_testing()
add_executable(tests test/tests.cpp)
target_link_libraries(tests gtest)

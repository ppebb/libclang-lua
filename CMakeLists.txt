cmake_minimum_required(VERSION 3.18)
project(libclang-lua CXX)

find_package(Lua REQUIRED)
find_library(CLANG_LIBRARY NAMES clang libclang
   HINTS /usr/lib/llvm18/lib/ /usr/lib/llvm-18/lib)
find_library(CLANG_CPP_LIBRARY NAMES clang-cpp libclang-cpp
   HINTS /usr/lib/llvm18/lib/ /usr/lib/llvm-18/lib)

if(APPLE)
   set(CMAKE_SHARED_MODULE_CREATE_CXX_FLAGS
       "${CMAKE_SHARED_MODULE_CREATE_CXX_FLAGS} -flat_namespace -undefined dynamic_lookup")
endif(APPLE)

include_directories(${LUA_INCLUDE_DIR})
add_library(libclang-lua MODULE libclang-lua.cpp)
target_link_libraries(libclang-lua ${CLANG_LIBRARY} ${CLANG_CPP_LIBRARY})
set_target_properties(libclang-lua PROPERTIES PREFIX "")

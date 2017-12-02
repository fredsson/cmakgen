cmake_minimum_required (VERSION 2.8.9)

project (cmake_generator)

if (NOT CMAKE_BUILD_TYPE)
  message ("-- No Build type set: defaulting to Debug")
  set (CMAKE_BUILD_TYPE Debug)
endif (NOT CMAKE_BUILD_TYPE)

include (CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORT_17)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORT_14)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORT_11)
if (COMPILER_SUPPORT_17)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
elseif (COMPILER_SUPPORT_14)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
elseif (COMPILER_SUPPORT_11)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif ()

add_library(stdc++fs UNKNOWN IMPORTED)
set_property(TARGET stdc++fs PROPERTY IMPORTED_LOCATION "/usr/lib/libstdc++fs.a")

set (CMAKE_GENERATOR_INCLUDE
  "include/cmakeproject.h"
  "include/cmakegenerator.h"
  "include/fileutils.h"
)

set (CMAKE_GENERATOR_SRC
  "src/main.cpp"
  "src/cmakeproject.cpp"
  "src/cmakegenerator.cpp"
  "src/fileutils.cpp"
)

add_executable (cmake_generator ${CMAKE_GENERATOR_INCLUDE} ${CMAKE_GENERATOR_SRC})

target_link_libraries(cmake_generator stdc++fs)
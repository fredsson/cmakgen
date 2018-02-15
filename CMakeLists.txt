cmake_minimum_required (VERSION 2.8.9)

project (cmakgen)

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

set (CMAKGEN_INCLUDE
  "include/parser/cmlistfilelexer.h"
  "include/cmakefile.h"
  "include/cmakefunction.h"
  "include/parser/cmakeparser.h"
  "include/writer/cmakewriter.h"
  "include/fileutils.h"
  "include/commandlineutils.h"
  "include/cmakegenerator.h"
)

set_source_files_properties(src/parser/cmlistfilelexer.c PROPERTIES COMPILE_FLAGS "-DYY_NO_INPUT -DYY_NO_UNPUT")

set (CMAKGEN_SRC
  "src/parser/cmlistfilelexer.c"
  "src/cmakefile.cpp"
  "src/cmakefunction.cpp"
  "src/parser/cmakeparser.cpp"
  "src/writer/cmakewriter.cpp"
  "src/fileutils.cpp"
  "src/commandlineutils.cpp"
  "src/cmakegenerator.cpp"
  "src/main.cpp"
)


add_executable (cmakgen ${CMAKGEN_INCLUDE} ${CMAKGEN_SRC})

set_target_properties (cmakgen PROPERTIES OUTPUT_NAME "cmakgen")
target_link_libraries(cmakgen stdc++fs)

install (
  TARGETS
  cmakgen
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static
)
cmake_minimum_required(VERSION 3.6.13)

project(Virtual_mapping)

set(SOURCE_EXE main.cpp)
set(SOURCE_CODE internal)

set(SOURCE_GRAPH ${SOURCE_CODE}/graph/graph.cpp)
set(SOURCE_PARSER ${SOURCE_CODE}/parser/parser.cpp)
set(SOURCE_SOLVER ${SOURCE_CODE}/solver/solver.cpp)

if ($ENV{DEBUG})
    set(ADD_DEBUG_FLAG -g)
    message("Debug mode enabled")
endif()


add_compile_options(-std=c++17 ${ADD_DEBUG_FLAG})
add_library(mapping_library STATIC ${SOURCE_GRAPH} ${SOURCE_PARSER} ${SOURCE_SOLVER} ${SOURCE_HEAP})
target_link_libraries(mapping_library -lgvc -lcgraph)
add_executable(main ${SOURCE_EXE})
target_link_libraries(main mapping_library)
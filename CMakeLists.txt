project(cocg)

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)

# Default exec directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}")

# default lib(static and dynamic) directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}")

# compilation mode: Debug or Release
set(CMAKE_BUILD_TYPE Debug)

option(OUTPUT_DEBUG_INFO "Output debug info" ON)
option(BUILD_TESTING "compile tests" ON)

if(OUTPUT_DEBUG_INFO)
    add_compile_definitions(OUTPUT_DEBUG_INFO)
endif()

link_directories("${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}")

add_subdirectory(cocg_ast)
add_subdirectory(cocg_pddl_parser)
add_subdirectory(cocg_core)
add_subdirectory(cocg_cff_plan_solver)
add_subdirectory(cocg_tests)


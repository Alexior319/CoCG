project(cocg)

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)

# Default exec directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}")

# default lib(static and dynamic) directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}")

add_subdirectory(cocg_ast)
add_subdirectory(cocg_pddl_parser)
add_subdirectory(cocg_core)


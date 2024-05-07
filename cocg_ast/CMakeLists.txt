project(cocg_pddl_parser)

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

install(DIRECTORY include/
  DESTINATION ${CMAKE_BINARY_DIR}/include
)

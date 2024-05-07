project(cocg_core)

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)

include_directories(
  include
  ${CMAKE_SOURCE_DIR}/cocg_ast/include
)

set(COCG_CORE_SRC
  src/utils.cpp
)

add_library(${PROJECT_NAME} SHARED ${COCG_CORE_SRC})

install(DIRECTORY include/
  DESTINATION ${CMAKE_BINARY_DIR}/include
)

install(TARGETS
  ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}
  LIBRARY DESTINATION ${CMAKE_BINARY_DIR}
  RUNTIME DESTINATION ${CMAKE_BINARY_DIR}
)

project(cocg_core)

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)

include_directories(
  include
  ${CMAKE_SOURCE_DIR}/cocg_ast/include
  ${CMAKE_SOURCE_DIR}/cocg_pddl_parser/include
)

set(COCG_CORE_SRC
  src/utils.cpp
  src/cocg_domain_expert.cpp
  src/cocg_domain_reader.cpp
  src/cocg_problem_expert_utils.cpp
  src/cocg_problem_expert.cpp
)

add_library(${PROJECT_NAME} SHARED ${COCG_CORE_SRC})
target_link_libraries(${PROJECT_NAME} cocg_pddl_parser)

install(DIRECTORY include/
  DESTINATION ${CMAKE_BINARY_DIR}/include
)

install(TARGETS
  ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}
  LIBRARY DESTINATION ${CMAKE_BINARY_DIR}
  RUNTIME DESTINATION ${CMAKE_BINARY_DIR}
)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()

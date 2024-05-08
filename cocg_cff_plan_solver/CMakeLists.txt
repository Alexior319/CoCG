project(cff_plan_solver)

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)

include_directories(
  include
  ${CMAKE_SOURCE_DIR}/cocg_ast/include
  ${CMAKE_SOURCE_DIR}/cocg_core/include
)

set(CFF_PLAN_SOLVER_SOURCES
  src/cff_plan_solver.cpp
)

add_library(${PROJECT_NAME} SHARED ${CFF_PLAN_SOLVER_SOURCES})
target_link_libraries(${PROJECT_NAME} cocg_core)

add_subdirectory(src/contingent-ff)

install(DIRECTORY include/
  DESTINATION ${CMAKE_BINARY_DIR}/include
)

install(TARGETS
  ${PROJECT_NAME}
  parser
  ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}
  LIBRARY DESTINATION ${CMAKE_BINARY_DIR}
  RUNTIME DESTINATION ${CMAKE_BINARY_DIR}
)

file(COPY
  test/pddl
  DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}
)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()


project(cocg_pddl_parser)

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)

set(dependencies

)

include_directories(
  include
  ${CMAKE_SOURCE_DIR}/cocg_ast/include
)

set(PDDL-PARSER-SOURCES
  src/cocg_pddl_parser/Action.cpp
  src/cocg_pddl_parser/And.cpp
  src/cocg_pddl_parser/Derived.cpp
  src/cocg_pddl_parser/Exists.cpp
  src/cocg_pddl_parser/Expression.cpp
  src/cocg_pddl_parser/Forall.cpp
  src/cocg_pddl_parser/Function.cpp
  src/cocg_pddl_parser/FunctionModifier.cpp
  src/cocg_pddl_parser/Ground.cpp
  src/cocg_pddl_parser/GroundFunc.cpp
  src/cocg_pddl_parser/Lifted.cpp
  src/cocg_pddl_parser/Not.cpp
  src/cocg_pddl_parser/Oneof.cpp
  src/cocg_pddl_parser/Or.cpp
  src/cocg_pddl_parser/ParamCond.cpp
  src/cocg_pddl_parser/TemporalAction.cpp
  src/cocg_pddl_parser/TypeGround.cpp
  src/cocg_pddl_parser/Unknown.cpp
  src/cocg_pddl_parser/Utils.cpp
  src/cocg_pddl_parser/When.cpp
)

add_library(${PROJECT_NAME} SHARED ${PDDL-PARSER-SOURCES})

add_executable(parser src/parser.cpp)
target_link_libraries(parser ${PROJECT_NAME})

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
  test/pddl_parser
  DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}
)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()


enable_testing()

add_executable(pddl_parser_test pddl_parser_test.cpp)
set_target_properties(pddl_parser_test PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

target_link_libraries(pddl_parser_test cocg_pddl_parser gtest gtest_main pthread)

add_test(
  NAME pddl_parser_test
  COMMAND pddl_parser_test
)

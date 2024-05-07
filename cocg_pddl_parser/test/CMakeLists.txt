enable_testing()

add_executable(pddl_parser_test pddl_parser_test.cpp)

target_link_libraries(pddl_parser_test cocg_pddl_parser gtest gtest_main)

add_test(
  NAME pddl_parser_test
  COMMAND pddl_parser_test
)

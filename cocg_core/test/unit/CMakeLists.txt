enable_testing()

add_executable(utils_test utils_test.cpp)
target_link_libraries(utils_test ${PROJECT_NAME} gtest gtest_main pthread)
add_test(
  NAME utils_test
  COMMAND utils_test
)

add_executable(problem_expert_test problem_expert_test.cpp)
target_link_libraries(problem_expert_test ${PROJECT_NAME} gtest gtest_main pthread)
add_test(
  NAME problem_expert_test
  COMMAND problem_expert_test
)

add_executable(domain_expert_test domain_expert_test.cpp)
target_link_libraries(domain_expert_test ${PROJECT_NAME} gtest gtest_main pthread)
add_test(
  NAME domain_expert_test
  COMMAND domain_expert_test
)

add_executable(domain_reader_test domain_reader_test.cpp)
target_link_libraries(domain_reader_test ${PROJECT_NAME} gtest gtest_main pthread)
add_test(
  NAME domain_reader_test
  COMMAND domain_reader_test
)

add_executable(types_test types_test.cpp)
target_link_libraries(types_test ${PROJECT_NAME} gtest gtest_main pthread)
add_test(
  NAME types_test
  COMMAND types_test
)


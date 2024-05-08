
enable_testing()

add_executable(cff_test cff_test.cpp)

target_link_libraries(cff_test ${PROJECT_NAME} cff_plan_solver gtest gtest_main pthread dl)

add_test(
  NAME cff_test
  COMMAND cff_test
)

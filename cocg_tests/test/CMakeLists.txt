include_directories(
  include
  ${CMAKE_SOURCE_DIR}/cocg_ast/include
  ${CMAKE_SOURCE_DIR}/cocg_pddl_parser/include
  ${CMAKE_SOURCE_DIR}/cocg_core/include
  ${CMAKE_SOURCE_DIR}/cocg_cff_plan_solver/include
)

enable_testing()

add_executable(cocg_core_test cocg_core_test.cpp)
target_link_libraries(cocg_core_test cocg_core cff_plan_solver gtest gtest_main pthread)
add_test(
  NAME cocg_core_test
  COMMAND cocg_core_test
)

add_executable(cocg_graphplan_test cocg_graphplan_test.cpp)
target_link_libraries(cocg_graphplan_test cocg_core cff_plan_solver gtest gtest_main pthread)
add_test(
  NAME cocg_graphplan_test
  COMMAND cocg_graphplan_test
)

add_executable(cff_cocg_test cff_cocg_test.cpp cff_cocg.cpp)
target_link_libraries(cff_cocg_test cocg_core cff_plan_solver)

add_executable(cff_cocg cff_cocg_main.cpp cff_cocg.cpp)
target_link_libraries(cff_cocg cocg_core cff_plan_solver)
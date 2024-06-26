

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "cocg_cff_plan_solver/cff_plan_solver.h"
#include "gtest/gtest.h"

class CFFPlanSolverTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {}
};

TEST(CFFPlanSolverTest, generate_plan_good) {
  std::string pkgpath = std::filesystem::current_path().string();
  std::ifstream domain_ifs(pkgpath + "/pddl/domain.pddl");
  std::string domain_str((std::istreambuf_iterator<char>(domain_ifs)),
                         std::istreambuf_iterator<char>());

  std::ifstream problem_ifs(pkgpath + "/pddl/problem_blocks.pddl");
  std::string problem_str((std::istreambuf_iterator<char>(problem_ifs)),
                          std::istreambuf_iterator<char>());

  auto planner = std::make_shared<cocg::CFFPlanSolver>();
  auto plan = planner->get_cocg_cont_plan_ast(domain_str, problem_str);

  ASSERT_TRUE(plan);
  ASSERT_EQ(plan.value().items.size(), 6);
  ASSERT_EQ(plan.value().items[0].action, "(senseon b2 b3)");
}

TEST(CFFPlanSolverTest, generate_plan_unsolvable) {
  std::string pkgpath = std::filesystem::current_path().string();
  std::ifstream domain_ifs(pkgpath + "/pddl/domain.pddl");
  std::string domain_str((std::istreambuf_iterator<char>(domain_ifs)),
                         std::istreambuf_iterator<char>());

  std::ifstream problem_ifs(pkgpath + "/pddl/problem_blocks_impossible.pddl");
  std::string problem_str((std::istreambuf_iterator<char>(problem_ifs)),
                          std::istreambuf_iterator<char>());

  auto planner = std::make_shared<cocg::CFFPlanSolver>();
  auto plan = planner->get_cocg_cont_plan_ast(domain_str, problem_str);

  ASSERT_FALSE(plan);
}

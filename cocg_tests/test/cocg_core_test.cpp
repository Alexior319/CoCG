
#include "cocg_core/cocg_core.h"

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "cocg_ast/tree.h"
#include "cocg_cff_plan_solver/cff_plan_solver.h"
#include "gtest/gtest.h"

class cocg_core_test : public ::testing::Test {
 protected:
  static void SetUpTestCase() {}
};

TEST(cocg_core_test, get_contingent_plan) {
  std::string pkgpath = std::filesystem::current_path().string();
  std::string domain_path = pkgpath + "/pddl/domain_blocks_observe.pddl";
  std::ifstream domain_ifs(domain_path);
  std::string domain_str((std::istreambuf_iterator<char>(domain_ifs)),
                         std::istreambuf_iterator<char>());

  std::string problem_path = pkgpath + "/pddl/problem_blocks_observe.pddl";
  std::ifstream problem_ifs(problem_path);
  std::string problem_str((std::istreambuf_iterator<char>(problem_ifs)),
                          std::istreambuf_iterator<char>());

  auto domain_expert = std::make_shared<cocg::DomainExpert>(domain_str);
  cocg::ProblemExpert problem_expert(domain_expert);

  auto cff_plan_sovler = std::make_shared<cocg::CFFPlanSolver>();

  auto root =
      cff_plan_sovler->get_cocg_cont_plan_tree_root(domain_str, problem_str);

  cocg::print_cont_plan_tree(root);
}

TEST(cocg_core_test, traverse_contingent_plan_tree) {
  std::string pkgpath = std::filesystem::current_path().string();
  std::string domain_path = pkgpath + "/pddl/domain_blocks_observe.pddl";
  std::ifstream domain_ifs(domain_path);
  std::string domain_str((std::istreambuf_iterator<char>(domain_ifs)),
                         std::istreambuf_iterator<char>());

  std::string problem_path = pkgpath + "/pddl/problem_blocks_observe.pddl";
  std::ifstream problem_ifs(problem_path);
  std::string problem_str((std::istreambuf_iterator<char>(problem_ifs)),
                          std::istreambuf_iterator<char>());

  auto domain_expert = std::make_shared<cocg::DomainExpert>(domain_str);
  auto problem_expert_init =
      std::make_shared<cocg::ProblemExpert>(domain_expert);
  problem_expert_init->addProblem(problem_str);

  auto cff_plan_sovler = std::make_shared<cocg::CFFPlanSolver>();

  auto root =
      cff_plan_sovler->get_cocg_cont_plan_tree_root(domain_str, problem_str);
  cocg::print_cont_plan_tree(root);
  std::cout << "-----------------------------" << std::endl;

  std::tuple<std::shared_ptr<cocg::ProblemExpert>,
             std::vector<cocg_ast::Action>, cocg::ContPlanNode::SharedPtr>
      ret = cocg::traverse_contingent_planning_tree(problem_expert_init, root,
                                                    *domain_expert);
  std::shared_ptr<cocg::ProblemExpert> goal_state1 = std::get<0>(ret);
  auto preds = goal_state1->getPredicates();
  std::string goal_state_str;
  for (auto pred : preds) {
    goal_state_str += parser::pddl::toString(pred) + " ";
  }
  std::cout << "init states/goal state1: " << goal_state_str << std::endl;

  std::vector<cocg_ast::Action> actions1 = std::get<1>(ret);
  cocg::ContPlanNode::SharedPtr last_node1 = std::get<2>(ret);
  ASSERT_EQ(problem_expert_init->getPredicates(), goal_state1->getPredicates());
  ASSERT_EQ(actions1.size(), 0);
  std::cout << "last node1: ";
  last_node1->print_info();
  cocg_ast::Action::SharedPtr last_node1_ast =
      cocg::convert_plan_node_to_ast(last_node1, *domain_expert);

  std::cout << "-----------------------------" << std::endl;
  std::shared_ptr<cocg::ProblemExpert> next_true_init_state =
      cocg::apply_sensing_action(goal_state1, *last_node1_ast, true, true);
  preds = next_true_init_state->getPredicates();
  std::string next_true_init_state_str;
  for (auto pred : preds) {
    next_true_init_state_str += parser::pddl::toString(pred) + " ";
  }
  std::cout << "next true init state: " << next_true_init_state_str
            << std::endl;
  cocg::ContPlanNode::SharedPtr next_true_node = last_node1->true_node;
  std::cout << "next true node after node1: ";
  next_true_node->print_info();

  std::cout << "\tcontinue with the true parts-----------" << std::endl;
  std::tuple<std::shared_ptr<cocg::ProblemExpert>,
             std::vector<cocg_ast::Action>, cocg::ContPlanNode::SharedPtr>
      ret_true = cocg::traverse_contingent_planning_tree(
          next_true_init_state, next_true_node, *domain_expert);
  std::shared_ptr<cocg::ProblemExpert> true_goal_state1 = std::get<0>(ret_true);
  auto true_preds = true_goal_state1->getPredicates();
  std::string true_goal_state_str;
  for (auto pred : true_preds) {
    true_goal_state_str += parser::pddl::toString(pred) + " ";
  }
  std::cout << "\tgoal state in the true branch: " << true_goal_state_str
            << std::endl;

  std::cout << "-----------------------------" << std::endl;
  std::shared_ptr<cocg::ProblemExpert> next_false_init_state =
      cocg::apply_sensing_action(goal_state1, *last_node1_ast, false, true);
  preds = next_false_init_state->getPredicates();
  std::string next_false_init_state_str;
  for (auto pred : preds) {
    next_false_init_state_str += parser::pddl::toString(pred) + " ";
  }
  std::cout << "next false init state: " << next_false_init_state_str
            << std::endl;
  cocg::ContPlanNode::SharedPtr next_false_node = last_node1->false_node;
  std::cout << "next false node after node1: ";
  next_false_node->print_info();
  std::cout << "\tcontinue with the false parts-----------" << std::endl;
  std::tuple<std::shared_ptr<cocg::ProblemExpert>,
             std::vector<cocg_ast::Action>, cocg::ContPlanNode::SharedPtr>
      ret_false = cocg::traverse_contingent_planning_tree(
          next_false_init_state, next_false_node, *domain_expert);
  std::shared_ptr<cocg::ProblemExpert> false_goal_state1 =
      std::get<0>(ret_false);
  auto false_preds = false_goal_state1->getPredicates();
  std::string false_goal_state_str;
  for (auto pred : false_preds) {
    false_goal_state_str += parser::pddl::toString(pred) + " ";
  }
  std::cout << "\tgoal state in the false branch: " << false_goal_state_str
            << std::endl;
}

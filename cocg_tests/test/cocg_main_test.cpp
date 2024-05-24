#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "cocg_ast/tree.h"
#include "cocg_cff_plan_solver/cff_plan_solver.h"
#include "cocg_core/cocg_core.h"

void test_blocks_r2p3b2() {
  std::string pkgpath = std::filesystem::current_path().string();
  std::string domain_path = pkgpath + "/pddl/cocg/d_blocks.pddl";
  std::ifstream domain_ifs(domain_path);
  std::string domain_str((std::istreambuf_iterator<char>(domain_ifs)),
                         std::istreambuf_iterator<char>());

  std::string problem_path = pkgpath + "/pddl/cocg/p_blocks_r2p3b2.pddl";
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

  auto init_preds = problem_expert_init->getPredicates();
  std::string init_state_str;
  for (auto pred : init_preds) {
    init_state_str += parser::pddl::toString(pred) + " ";
  }
  std::cout << "init state: " << init_state_str << std::endl;
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
  std::cout << "goal state1: " << goal_state_str << std::endl;

  std::vector<cocg_ast::Action> actions1 = std::get<1>(ret);
  cocg::ContPlanNode::SharedPtr last_node1 = std::get<2>(ret);
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

  std::cout << "------------continue with the true parts-----------"
            << std::endl;
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
  std::cout << "-------------continue with the false parts-----------"
            << std::endl;
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

void test_cocg_method() {
  std::string pkgpath = std::filesystem::current_path().string();
  std::string domain_path = pkgpath + "/pddl/cocg/d_blocks.pddl";
  std::ifstream domain_ifs(domain_path);
  std::string domain_str((std::istreambuf_iterator<char>(domain_ifs)),
                         std::istreambuf_iterator<char>());

  std::string problem_path = pkgpath + "/pddl/cocg/p_blocks_r2p3b2.pddl";
  std::ifstream problem_ifs(problem_path);
  std::string problem_str((std::istreambuf_iterator<char>(problem_ifs)),
                          std::istreambuf_iterator<char>());

  auto domain_expert = std::make_shared<cocg::DomainExpert>(domain_str);
  auto problem_expert_init =
      std::make_shared<cocg::ProblemExpert>(domain_expert);
  problem_expert_init->addProblem(problem_str);

  auto cff_plan_sovler = std::make_shared<cocg::CFFPlanSolver>();

  auto cff_plan_tree_root =
      cff_plan_sovler->get_cocg_cont_plan_tree_root(domain_str, problem_str);
  cocg::print_cont_plan_tree(cff_plan_tree_root);
  std::cout << "-----------------------------" << std::endl;

  uint32_t t0 = 0;

  auto ret = cocg::traverse_contingent_planning_tree(
      problem_expert_init, cff_plan_tree_root, *domain_expert);
  auto root_goal_state = std::get<0>(ret);
  auto root_actions_set = std::get<1>(ret);
  auto root_next_node = std::get<2>(ret);

  auto cocg_subgraph_root = cocg::build_cocg_subgraph(
      problem_expert_init, root_goal_state, root_actions_set, root_next_node,
      t0, *domain_expert);
  cocg::print_cocg_graph(cocg_subgraph_root);
}

int main() {
  //   test_blocks_r2p3b2();
  test_cocg_method();
  return 0;
}

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

  //   cocg_ast::Node on_table_b1;
  //   on_table_b1.node_type = cocg_ast::Node::PREDICATE;
  //   on_table_b1.name = "ontable";
  //   on_table_b1.parameters.push_back(
  //       parser::pddl::fromStringParam("b1", "block"));

  //   cocg_ast::Node clear_b1;
  //   clear_b1.node_type = cocg_ast::Node::PREDICATE;
  //   clear_b1.name = "clear";
  //   clear_b1.parameters.push_back(parser::pddl::fromStringParam("b1",
  //   "block"));

  //   cocg_ast::Node on_table_b3;
  //   on_table_b3.node_type = cocg_ast::Node::PREDICATE;
  //   on_table_b3.name = "ontable";
  //   on_table_b3.parameters.push_back(
  //       parser::pddl::fromStringParam("b3", "block"));

  //   cocg_ast::Node clear_b2;
  //   clear_b2.node_type = cocg_ast::Node::PREDICATE;
  //   clear_b2.name = "clear";
  //   clear_b2.parameters.push_back(parser::pddl::fromStringParam("b2",
  //   "block"));

  //   ASSERT_TRUE(
  //       problem_expert.addInstance(parser::pddl::fromStringParam("b1",
  //       "block")));
  //   ASSERT_TRUE(
  //       problem_expert.addInstance(parser::pddl::fromStringParam("b2",
  //       "block")));
  //   ASSERT_TRUE(
  //       problem_expert.addInstance(parser::pddl::fromStringParam("b3",
  //       "block")));

  //   auto unknown_cond_1 = cocg::Unknown("(unknown (ontable b2))");
  //   auto unknown_cond_2 = cocg::Unknown("(unknown (on b2 b3))");
  //   ASSERT_TRUE(problem_expert.addConditional(unknown_cond_1));
  //   ASSERT_TRUE(problem_expert.addConditional(unknown_cond_2));

  //   auto oneof_cond = cocg::OneOf("(oneof (ontable b2) (on b2 b3))");
  //   ASSERT_TRUE(problem_expert.addConditional(oneof_cond));

  //   auto or_cond = cocg::Or("(or (on b2 b3) (on b3 b2))");
  //   ASSERT_TRUE(problem_expert.addConditional(or_cond));
  //   ASSERT_TRUE(problem_expert.removeConditional(or_cond));
  //   ASSERT_TRUE(problem_expert.addConditional(or_cond));

  //   auto or_cond2 = cocg::Or("(or (not (clear b1)) (clear b1))");
  //   ASSERT_TRUE(problem_expert.addConditional(or_cond2));
  //   ASSERT_TRUE(problem_expert.removeConditional(or_cond2));
  //   ASSERT_TRUE(problem_expert.addConditional(or_cond2));
  //   ASSERT_TRUE(problem_expert.addConditional(or_cond2));
  //   ASSERT_TRUE(problem_expert.addPredicate(on_table_b1));
  //   ASSERT_TRUE(problem_expert.addPredicate(clear_b1));
  //   ASSERT_TRUE(problem_expert.addPredicate(on_table_b3));
  //   ASSERT_TRUE(problem_expert.addPredicate(clear_b2));
  //   ASSERT_TRUE(problem_expert.removePredicate(clear_b2));

  //   cocg_ast::Tree goal;
  //   parser::pddl::fromString(goal, "(and (on b1 b2))");
  //   ASSERT_TRUE(problem_expert.setGoal(goal));

  //   auto tmp = std::string(
  //                  "( define ( problem problem_1 )\n( :domain blocksworld
  //                  )\n( "
  //                  ":objects\n\tb1 b2 b3 ") +
  //              std::string(
  //                  "- block\n)\n( :init\n\t( ontable b1 )\n\t( clear b1
  //                  )\n\t( " "ontable b3 )\n\t( unknown ( ontable ") +
  //              std::string(
  //                  "b2 )\t)\n\t( unknown ( on b2 b3 )\t)\n\t( oneof\n\t\t( "
  //                  "ontable b2 )\n\t\t( on b2 b3 )\n\t)\n\t( or") +
  //              std::string(
  //                  "\n\t\t( on b2 b3 )\n\t\t( on b3 b2 )\n\t)\n\t( or\n\t\t(
  //                  not "
  //                  "( clear b1 ) )\n\t\t( clear b1 )\n\t)\n)\n(
  //                  :goal\n\t\t\t( " "on b1 b2 )\n\t)\n)\n");
  //   auto problem_str = problem_expert.getProblem();
  //   std::cout << problem_str;
  //   ASSERT_EQ(problem_str, tmp);

  //   ASSERT_TRUE(problem_expert.clearKnowledge());
  //   ASSERT_EQ(problem_expert.getPredicates().size(), 0);
  //   ASSERT_EQ(problem_expert.getFunctions().size(), 0);
  //   ASSERT_EQ(problem_expert.getInstances().size(), 0);
  //   ASSERT_EQ(problem_expert.getConditionals().size(), 0);
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
  auto problem_expert =
      std::make_shared<cocg::ProblemExpert>(problem_expert_init);

  auto cff_plan_sovler = std::make_shared<cocg::CFFPlanSolver>();

  auto root =
      cff_plan_sovler->get_cocg_cont_plan_tree_root(domain_str, problem_str);
  cocg::print_cont_plan_tree(root);
  std::cout << "-----------------------------" << std::endl;

  std::tuple<std::shared_ptr<cocg::ProblemExpert>,
             std::vector<cocg_ast::Action::SharedPtr>,
             cocg::ContPlanNode::SharedPtr>
      ret = cocg::traverse_contingent_planning_tree(problem_expert, root,
                                                    *domain_expert);
  std::shared_ptr<cocg::ProblemExpert> goal_state1 = std::get<0>(ret);
  auto preds = goal_state1->getPredicates();
  std::string goal_state_str;
  for (auto pred : preds) {
    goal_state_str += parser::pddl::toString(pred) + " ";
  }
  std::cout << "init states/goal state1: " << goal_state_str << std::endl;

  std::vector<cocg_ast::Action::SharedPtr> actions1 = std::get<1>(ret);
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
}

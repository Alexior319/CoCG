
#include "cocg_pddl_parser/Utils.h"

#include <map>
#include <memory>
#include <set>
#include <string>
#include <tuple>
#include <vector>
#include <filesystem>

#include "cocg_ast/knowledge.h"
#include "cocg_ast/node.h"
#include "cocg_ast/param.h"
#include "cocg_ast/tree.h"
#include "cocg_core/cocg_domain_expert.h"
#include "cocg_core/cocg_problem_expert.h"
#include "cocg_core/cocg_problem_expert_utils.h"
#include "gtest/gtest.h"

class utils : public ::testing::Test {
 protected:
  static void SetUpTestCase() {}
};

TEST(utils, evaluate_and)
{
  std::vector<cocg::Predicate> predicates;
  std::vector<cocg::Function> functions;
  auto problem_client = std::make_shared<cocg::ProblemExpert>();

  std::string expression = "(and (patrolled wp1) (patrolled wp2))";
  cocg_ast::Tree goal;
  parser::pddl::fromString(goal, expression);

  ASSERT_EQ(
    cocg::evaluate(goal, problem_client, predicates, functions, false, true),
    std::make_tuple(true, false, 0));

  predicates.push_back(parser::pddl::fromStringPredicate("(patrolled wp1)"));

  ASSERT_EQ(
    cocg::evaluate(goal, problem_client, predicates, functions, false, true),
    std::make_tuple(true, false, 0));

  predicates.clear();
  predicates.push_back(parser::pddl::fromStringPredicate("(patrolled wp2)"));

  ASSERT_EQ(
    cocg::evaluate(goal, problem_client, predicates, functions, false, true),
    std::make_tuple(true, false, 0));

  predicates.push_back(parser::pddl::fromStringPredicate("(patrolled wp1)"));

  ASSERT_EQ(
    cocg::evaluate(goal, problem_client, predicates, functions, false, true),
    std::make_tuple(true, true, 0));
}

TEST(utils, evaluate_or)
{
  std::vector<cocg::Predicate> predicates;
  std::vector<cocg::Function> functions;
  auto problem_client = std::make_shared<cocg::ProblemExpert>();

  cocg_ast::Tree test_tree;
  parser::pddl::fromString(
    test_tree, "(or (patrolled wp1) (patrolled wp2))", false,
    cocg_ast::Node::AND);

  ASSERT_EQ(
    cocg::evaluate(test_tree, problem_client, predicates, functions, false, true),
    std::make_tuple(true, false, 0));

  predicates.push_back(parser::pddl::fromStringPredicate("(patrolled wp1)"));

  ASSERT_EQ(
    cocg::evaluate(test_tree, problem_client, predicates, functions, false, true),
    std::make_tuple(true, true, 0));

  predicates.clear();
  predicates.push_back(parser::pddl::fromStringPredicate("(patrolled wp2)"));

  ASSERT_EQ(
    cocg::evaluate(test_tree, problem_client, predicates, functions, false, true),
    std::make_tuple(true, true, 0));

  predicates.push_back(parser::pddl::fromStringPredicate("(patrolled wp1)"));

  ASSERT_EQ(
    cocg::evaluate(test_tree, problem_client, predicates, functions, false, true),
    std::make_tuple(true, true, 0));
}

TEST(utils, evaluate_not)
{
  std::vector<cocg::Predicate> predicates;
  std::vector<cocg::Function> functions;
  auto problem_client = std::make_shared<cocg::ProblemExpert>();

  cocg_ast::Tree test_tree;
  parser::pddl::fromString(
    test_tree, "(not (patrolled wp1))", false,
    cocg_ast::Node::AND);

  ASSERT_EQ(
    cocg::evaluate(test_tree, problem_client, predicates, functions, false, true),
    std::make_tuple(true, true, 0));

  predicates.push_back(parser::pddl::fromStringPredicate("(patrolled wp1)"));

  ASSERT_EQ(
    cocg::evaluate(test_tree, problem_client, predicates, functions, false, true),
    std::make_tuple(true, false, 0));
}

TEST(utils, evaluate_predicate_use_state)
{
  std::vector<cocg::Predicate> predicates;
  std::vector<cocg::Function> functions;
  auto problem_client = std::make_shared<cocg::ProblemExpert>();

  cocg_ast::Tree test_tree;
  parser::pddl::fromString(test_tree, "(patrolled wp1)", false, cocg_ast::Node::AND);

  ASSERT_EQ(
    cocg::evaluate(test_tree, problem_client, predicates, functions, false, true),
    std::make_tuple(true, false, 0));

  ASSERT_EQ(
    cocg::evaluate(test_tree, problem_client, predicates, functions, false, true, 0, true),
    std::make_tuple(true, true, 0));

  ASSERT_TRUE(cocg::apply(test_tree, predicates, functions));
  ASSERT_EQ(predicates.size(), 1);
  ASSERT_EQ(parser::pddl::toString(*predicates.begin()), "(patrolled wp1)");

  ASSERT_EQ(
    cocg::evaluate(test_tree, problem_client, predicates, functions, false, true),
    std::make_tuple(true, true, 0));

  ASSERT_EQ(
    cocg::evaluate(test_tree, problem_client, predicates, functions, false, true, 0, true),
    std::make_tuple(true, false, 0));

  ASSERT_EQ(
    cocg::evaluate(test_tree, problem_client, predicates, functions, true, true, 0, true),
    std::make_tuple(true, false, 0));
  ASSERT_TRUE(predicates.empty());
}

TEST(utils, evaluate_function_use_state)
{
  std::vector<cocg::Predicate> predicates;
  std::vector<cocg::Function> functions;

  cocg_ast::Tree test_tree;
  parser::pddl::fromString(
    test_tree, "(distance wp1 wp2)", false,
    cocg_ast::Node::EXPRESSION);

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(false, false, 0.0));

  functions.push_back(parser::pddl::fromStringFunction("(= (distance wp1 wp2) 1.0)"));

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(true, false, 1.0));
}

TEST(utils, evaluate_expression_ge)
{
  std::vector<cocg::Predicate> predicates;
  std::vector<cocg::Function> functions;
  auto problem_client = std::make_shared<cocg::ProblemExpert>();

  cocg_ast::Tree test_tree;
  parser::pddl::fromString(test_tree, "(>= (vx) 3.0)", false, cocg_ast::Node::EXPRESSION);

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(false, false, 0));

  functions.push_back(parser::pddl::fromStringFunction("(= (vx) 2.9999)"));

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(true, false, 0));

  functions[0].value = 4.0;

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(true, true, 0));

  functions[0].value = 3.0;

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(true, true, 0));
}

TEST(utils, evaluate_expression_gt)
{
  std::vector<cocg::Predicate> predicates;
  std::vector<cocg::Function> functions;
  auto problem_client = std::make_shared<cocg::ProblemExpert>();

  cocg_ast::Tree test_tree;
  parser::pddl::fromString(
    test_tree, "(> (distance wp1 wp2) 3.0)", false,
    cocg_ast::Node::AND);

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(false, false, 0));

  functions.push_back(parser::pddl::fromStringFunction("(= (distance wp1 wp2) 3.0)"));

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(true, false, 0));

  functions[0].value = 3.00001;

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(true, true, 0));
}

TEST(utils, evaluate_expression_le)
{
  std::vector<cocg::Predicate> predicates;
  std::vector<cocg::Function> functions;
  auto problem_client = std::make_shared<cocg::ProblemExpert>();

  cocg_ast::Tree test_tree;
  parser::pddl::fromString(
    test_tree, "(<= (vx) -3.0)", false,
    cocg_ast::Node::EXPRESSION);

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(false, false, 0));

  functions.push_back(parser::pddl::fromStringFunction("(= (vx) -2.9999)"));

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(true, false, 0));

  functions[0].value = -4.0;

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(true, true, 0));

  functions[0].value = -3.0;

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(true, true, 0));
}

TEST(utils, evaluate_expression_lt)
{
  std::vector<cocg::Predicate> predicates;
  std::vector<cocg::Function> functions;
  auto problem_client = std::make_shared<cocg::ProblemExpert>();

  cocg_ast::Tree test_tree;
  parser::pddl::fromString(
    test_tree, "(< (distance wp1 wp2) -3.0)", false,
    cocg_ast::Node::AND);

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(false, false, 0));

  functions.push_back(parser::pddl::fromStringFunction("(= (distance wp1 wp2) -3.0)"));

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(true, false, 0));

  functions[0].value = -3.00001;

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(true, true, 0));
}

TEST(utils, evaluate_expression_multiply)
{
  std::vector<cocg::Predicate> predicates;
  std::vector<cocg::Function> functions;
  auto problem_client = std::make_shared<cocg::ProblemExpert>();

  cocg_ast::Tree test_tree;
  parser::pddl::fromString(test_tree, "(* (vx) 3.0)", false, cocg_ast::Node::AND);

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(false, false, 0));

  functions.push_back(parser::pddl::fromStringFunction("(= (vx) 3.0)"));

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(true, false, 9.0));

  functions[0].value = -0.001;

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(true, false, -0.003));
}

TEST(utils, evaluate_expression_divide)
{
  std::vector<cocg::Predicate> predicates;
  std::vector<cocg::Function> functions;
  auto problem_client = std::make_shared<cocg::ProblemExpert>();

  cocg_ast::Tree test_tree;
  parser::pddl::fromString(test_tree, "(/ (vx) 3.0)", false, cocg_ast::Node::AND);

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(false, false, 0));

  functions.push_back(parser::pddl::fromStringFunction("(= (vx) 3.0)"));

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(true, false, 1.0));

  functions[0].value = -9.0;

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(true, false, -3.0));

  // Divide by zero
  test_tree.nodes.clear();
  parser::pddl::fromString(test_tree, "(/ (vx) 0)", false, cocg_ast::Node::AND);

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(false, false, 0));
}

TEST(utils, evaluate_expression_add)
{
  std::vector<cocg::Predicate> predicates;
  std::vector<cocg::Function> functions;
  auto problem_client = std::make_shared<cocg::ProblemExpert>();

  cocg_ast::Tree test_tree;
  parser::pddl::fromString(test_tree, "(+ (vx) 3.0)", false, cocg_ast::Node::AND);

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(false, false, 0));

  functions.push_back(parser::pddl::fromStringFunction("(= (vx) 3.0)"));

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(true, false, 6.0));

  functions[0].value = -0.001;

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(true, false, 2.999));
}

TEST(utils, evaluate_expression_subtract)
{
  std::vector<cocg::Predicate> predicates;
  std::vector<cocg::Function> functions;
  auto problem_client = std::make_shared<cocg::ProblemExpert>();

  cocg_ast::Tree test_tree;
  parser::pddl::fromString(test_tree, "(- (vx) 3.0)", false, cocg_ast::Node::AND);

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(false, false, 0));

  functions.push_back(parser::pddl::fromStringFunction("(= (vx) 2.5)"));

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(true, false, -0.5));

  functions[0].value = -0.001;

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(true, false, -3.001));
}

TEST(utils, evaluate_expression_invalid)
{
  std::vector<cocg::Predicate> predicates;
  std::vector<cocg::Function> functions;
  auto problem_client = std::make_shared<cocg::ProblemExpert>();

  // Unknown expression type
  cocg_ast::Tree test_tree;
  parser::pddl::fromString(test_tree, "(> (vx) 0)", false, cocg_ast::Node::AND);
  test_tree.nodes[0].expression_type = cocg_ast::Node::NONE;

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(false, false, 0));
}

TEST(utils, evaluate_function_mod)
{
  std::vector<cocg_ast::Node> predicates_msg;
  std::vector<cocg_ast::Node> functions_msg;

  auto problem_client = std::make_shared<cocg::ProblemExpert>();

  cocg_ast::Tree test_tree;
  parser::pddl::fromString(
    test_tree, "(assign (vx) 3.0)", false,
    cocg_ast::Node::EXPRESSION);

  parser::pddl::getPredicates(predicates_msg, test_tree);
  parser::pddl::getFunctions(functions_msg, test_tree);

  auto predicates = cocg::convertVector<cocg::Predicate, cocg_ast::Node>(
    predicates_msg);
  auto functions = cocg::convertVector<cocg::Function, cocg_ast::Node>(
    functions_msg);

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(true, false, 3.0));
  ASSERT_EQ(functions[0].value, 0.0);

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions, true),
    std::make_tuple(true, false, 3.0));
  ASSERT_EQ(functions[0].value, 3.0);

  test_tree.nodes.clear();
  parser::pddl::fromString(
    test_tree, "(increase (vx) 3.0)", false,
    cocg_ast::Node::EXPRESSION);

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions, true),
    std::make_tuple(true, false, 6.0));
  ASSERT_EQ(functions[0].value, 6.0);

  test_tree.nodes.clear();
  parser::pddl::fromString(
    test_tree, "(decrease (vx) 3.0)", false,
    cocg_ast::Node::EXPRESSION);

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions, true),
    std::make_tuple(true, false, 3.0));
  ASSERT_EQ(functions[0].value, 3.0);

  test_tree.nodes.clear();
  parser::pddl::fromString(
    test_tree, "(scale-up (vx) 3.0)", false,
    cocg_ast::Node::EXPRESSION);

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions, true),
    std::make_tuple(true, false, 9.0));
  ASSERT_EQ(functions[0].value, 9.0);

  test_tree.nodes.clear();
  parser::pddl::fromString(
    test_tree, "(scale-down (vx) 3.0)", false,
    cocg_ast::Node::EXPRESSION);

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions, true),
    std::make_tuple(true, false, 3.0));
  ASSERT_EQ(functions[0].value, 3.0);

  // divide by zero
  test_tree.nodes.clear();
  parser::pddl::fromString(
    test_tree, "(scale-down (vx) 0.0)", false,
    cocg_ast::Node::EXPRESSION);

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions, true),
    std::make_tuple(false, false, 0.0));
  ASSERT_EQ(functions[0].value, 3.0);
}

TEST(utils, evaluate_function_mod_invalid)
{
  std::vector<cocg::Predicate> predicates;
  std::vector<cocg::Function> functions;
  auto problem_client = std::make_shared<cocg::ProblemExpert>();

  // Unknown function modifier type
  cocg_ast::Tree test_tree;
  parser::pddl::fromString(
    test_tree, "(assign (vx) 3.0)", false,
    cocg_ast::Node::EXPRESSION);
  test_tree.nodes[0].node_type = cocg_ast::Node::NONE;

  ASSERT_EQ(
    cocg::evaluate(test_tree, predicates, functions),
    std::make_tuple(false, false, 0));
}

TEST(utils, evaluate_number)
{
  std::vector<cocg::Predicate> predicates;
  std::vector<cocg::Function> functions;
  auto problem_client = std::make_shared<cocg::ProblemExpert>();

  cocg_ast::Tree test_tree;
  parser::pddl::fromString(test_tree, "3.0", false, cocg_ast::Node::EXPRESSION);

  ASSERT_EQ(
    cocg::evaluate(test_tree, problem_client, predicates, functions),
    std::make_tuple(true, true, 3.0));
}

TEST(utils, evaluate_invalid)
{
  std::vector<cocg::Predicate> predicates;
  std::vector<cocg::Function> functions;
  auto problem_client = std::make_shared<cocg::ProblemExpert>();

  cocg_ast::Tree test_tree;
  ASSERT_EQ(
    cocg::evaluate(test_tree, problem_client, predicates, functions),
    std::make_tuple(true, true, 0));

  parser::pddl::fromString(test_tree, "(patrolled wp1)", false, cocg_ast::Node::AND);
  test_tree.nodes.front().node_type = cocg_ast::Node::NONE;

  ASSERT_EQ(
    cocg::evaluate(test_tree, problem_client, predicates, functions),
    std::make_tuple(false, false, 0));
}

TEST(utils, get_subtrees)
{
  std::vector<uint32_t> empty_expected;

  cocg_ast::Tree invalid_goal;
  ASSERT_EQ(parser::pddl::getSubtreeIds(invalid_goal), empty_expected);

  parser::pddl::fromString(invalid_goal, "(or (patrolled wp1) (patrolled wp2))");
  ASSERT_EQ(parser::pddl::getSubtreeIds(invalid_goal), empty_expected);

  std::vector<uint32_t> expected;
  expected.push_back(1);
  expected.push_back(2);

  cocg_ast::Tree goal;
  parser::pddl::fromString(goal, "(and (patrolled wp1) (patrolled wp2))");
  auto actual = parser::pddl::getSubtreeIds(goal);
  ASSERT_EQ(actual.size(), expected.size());
  for (size_t i = 0; i < expected.size(); i++) {
    ASSERT_EQ(actual[i], expected[i]);
  }
}

TEST(utils, get_params)
{
  std::string action_str = "(move r2d2 bedroom)";

  std::vector<std::string> expected;
  expected.push_back("r2d2");
  expected.push_back("bedroom");

  ASSERT_EQ(cocg::get_action_params(action_str), expected);
}

TEST(utils, get_name)
{
  std::string action_str = "(move r2d2 bedroom)";

  ASSERT_EQ(cocg::get_action_name(action_str), "move");
}

int main(int argc, char ** argv)
{
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}

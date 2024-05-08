
#include <string>
#include <vector>
#include <memory>

#include "gtest/gtest.h"
#include "cocg_pddl_parser/Utils.h"

#include "cocg_ast/node.h"
#include "cocg_ast/param.h"

class domain_types : public ::testing::Test {
 protected:
  static void SetUpTestCase() {}
};

TEST(domain_types, basic_types)
{
  cocg_ast::Node predicate_1;
  predicate_1.node_type = cocg_ast::Node::PREDICATE;
  predicate_1.name = "robot_at";
  predicate_1.parameters.push_back(parser::pddl::fromStringParam("r2d2", "robot"));
  predicate_1.parameters.push_back(parser::pddl::fromStringParam("bedroom", "room"));

  ASSERT_EQ(parser::pddl::toString(predicate_1), "(robot_at r2d2 bedroom)");
}

TEST(domain_types, predicate_tree_to_string)
{
  cocg_ast::Node predicate_1;
  predicate_1.node_type = cocg_ast::Node::PREDICATE;
  predicate_1.node_id = 0;
  predicate_1.name = "robot_at";
  predicate_1.parameters.push_back(parser::pddl::fromStringParam("r2d2", "robot"));
  predicate_1.parameters.push_back(parser::pddl::fromStringParam("bedroom", "room"));
  predicate_1.negate = false;

  cocg_ast::Node predicate_2;
  predicate_2.node_type = cocg_ast::Node::PREDICATE;
  predicate_2.node_id = 0;
  predicate_2.name = "robot_at";
  predicate_2.parameters.push_back(parser::pddl::fromStringParam("r2d2", "robot"));
  predicate_2.parameters.push_back(parser::pddl::fromStringParam("kitchen", "room"));
  predicate_2.negate = false;

  cocg_ast::Node predicate_3;
  predicate_3.node_type = cocg_ast::Node::PREDICATE;
  predicate_3.node_id = 0;
  predicate_3.name = "person_at";
  predicate_3.parameters.push_back(parser::pddl::fromStringParam("paco", "person"));
  predicate_3.parameters.push_back(parser::pddl::fromStringParam("bedroom", "room"));
  predicate_3.negate = false;

  cocg_ast::Node predicate_4;
  predicate_4.node_type = cocg_ast::Node::PREDICATE;
  predicate_4.node_id = 0;
  predicate_4.name = "person_at";
  predicate_4.parameters.push_back(parser::pddl::fromStringParam("paco", "person"));
  predicate_4.parameters.push_back(parser::pddl::fromStringParam("kitchen", "room"));
  predicate_4.negate = false;

  cocg_ast::Tree pn_1;
  pn_1.nodes.push_back(predicate_1);

  ASSERT_EQ(parser::pddl::toString(pn_1), "(robot_at r2d2 bedroom)");

  cocg_ast::Tree pn_2;
  pn_2.nodes.push_back(predicate_2);

  ASSERT_EQ(parser::pddl::toString(pn_2), "(robot_at r2d2 kitchen)");

  cocg_ast::Tree pn_3;
  pn_3.nodes.push_back(predicate_3);

  ASSERT_EQ(parser::pddl::toString(pn_3), "(person_at paco bedroom)");

  cocg_ast::Tree pn_4;
  pn_4.nodes.push_back(predicate_4);

  ASSERT_EQ(parser::pddl::toString(pn_4), "(person_at paco kitchen)");

  std::vector<cocg_ast::Tree> not_subtrees;
  not_subtrees.push_back(pn_2);
  auto not_tree = parser::pddl::fromSubtrees(not_subtrees, cocg_ast::Node::NOT);

  ASSERT_EQ(parser::pddl::toString(*not_tree), "(not (robot_at r2d2 kitchen))");

  std::vector<cocg_ast::Tree> or_subtrees;
  or_subtrees.push_back(pn_3);
  or_subtrees.push_back(pn_4);
  auto or_tree = parser::pddl::fromSubtrees(or_subtrees, cocg_ast::Node::OR);

  ASSERT_EQ(
    parser::pddl::toString(
      *or_tree), "(or (person_at paco bedroom)(person_at paco kitchen))");

  std::vector<cocg_ast::Tree> and_subtrees;
  and_subtrees.push_back(pn_1);
  and_subtrees.push_back(*not_tree);
  and_subtrees.push_back(*or_tree);
  auto and_tree = parser::pddl::fromSubtrees(and_subtrees, cocg_ast::Node::AND);

  ASSERT_EQ(
    parser::pddl::toString(*and_tree), std::string("(and (robot_at r2d2 bedroom)(not ") +
    std::string("(robot_at r2d2 kitchen))(or (person_at paco bedroom)(person_at paco kitchen)))"));
}

TEST(domain_types, predicate_tree_to_string_2)
{
  cocg_ast::Node predicate_1;
  predicate_1.node_type = cocg_ast::Node::PREDICATE;
  predicate_1.node_id = 0;
  predicate_1.name = "robot_at";
  predicate_1.parameters.push_back(parser::pddl::fromStringParam("r2d2", "robot"));
  predicate_1.parameters.push_back(parser::pddl::fromStringParam("bedroom", "room"));
  predicate_1.negate = false;

  cocg_ast::Node predicate_2;
  predicate_2.node_type = cocg_ast::Node::PREDICATE;
  predicate_2.node_id = 0;
  predicate_2.name = "robot_at";
  predicate_2.parameters.push_back(parser::pddl::fromStringParam("r2d2", "robot"));
  predicate_2.parameters.push_back(parser::pddl::fromStringParam("kitchen", "room"));
  predicate_2.negate = false;

  cocg_ast::Node predicate_3;
  predicate_3.node_type = cocg_ast::Node::PREDICATE;
  predicate_3.node_id = 0;
  predicate_3.name = "person_at";
  predicate_3.parameters.push_back(parser::pddl::fromStringParam("paco", "person"));
  predicate_3.parameters.push_back(parser::pddl::fromStringParam("bedroom", "room"));
  predicate_3.negate = false;

  cocg_ast::Node predicate_4;
  predicate_4.node_type = cocg_ast::Node::PREDICATE;
  predicate_4.node_id = 0;
  predicate_4.name = "person_at";
  predicate_4.parameters.push_back(parser::pddl::fromStringParam("paco", "person"));
  predicate_4.parameters.push_back(parser::pddl::fromStringParam("kitchen", "room"));
  predicate_4.negate = false;

  cocg_ast::Tree pn_1;
  pn_1.nodes.push_back(predicate_1);

  ASSERT_EQ(parser::pddl::toString(pn_1), "(robot_at r2d2 bedroom)");

  cocg_ast::Tree pn_2;
  pn_2.nodes.push_back(predicate_2);

  ASSERT_EQ(parser::pddl::toString(pn_2), "(robot_at r2d2 kitchen)");

  cocg_ast::Tree pn_3;
  pn_3.nodes.push_back(predicate_3);

  ASSERT_EQ(parser::pddl::toString(pn_3), "(person_at paco bedroom)");

  cocg_ast::Tree pn_4;
  pn_4.nodes.push_back(predicate_4);

  ASSERT_EQ(parser::pddl::toString(pn_4), "(person_at paco kitchen)");

  std::vector<cocg_ast::Tree> not_subtrees;
  not_subtrees.push_back(pn_2);
  auto not_tree = parser::pddl::fromSubtrees(not_subtrees, cocg_ast::Node::NOT);

  ASSERT_EQ(parser::pddl::toString(*not_tree), "(not (robot_at r2d2 kitchen))");

  std::vector<cocg_ast::Tree> not_subtrees_2;
  not_subtrees_2.push_back(*not_tree);
  auto not_tree_2 = parser::pddl::fromSubtrees(not_subtrees_2, cocg_ast::Node::NOT);

  ASSERT_EQ(parser::pddl::toString(*not_tree_2), "(robot_at r2d2 kitchen)");

  std::vector<cocg_ast::Tree> or_subtrees;
  or_subtrees.push_back(pn_3);
  or_subtrees.push_back(pn_4);
  auto or_tree = parser::pddl::fromSubtrees(or_subtrees, cocg_ast::Node::OR);

  ASSERT_EQ(
    parser::pddl::toString(
      *or_tree), "(or (person_at paco bedroom)(person_at paco kitchen))");

  std::vector<cocg_ast::Tree> and_subtrees;
  and_subtrees.push_back(pn_1);
  and_subtrees.push_back(*not_tree_2);
  and_subtrees.push_back(*or_tree);
  auto and_tree = parser::pddl::fromSubtrees(and_subtrees, cocg_ast::Node::AND);

  ASSERT_EQ(
    parser::pddl::toString(*and_tree), std::string("(and (robot_at r2d2 bedroom)") +
    std::string("(robot_at r2d2 kitchen)(or (person_at paco bedroom)(person_at paco kitchen)))"));
}

TEST(domain_types, predicate_tree_to_string_3)
{
  cocg_ast::Node predicate_1;
  predicate_1.node_type = cocg_ast::Node::PREDICATE;
  predicate_1.node_id = 0;
  predicate_1.name = "robot_at";
  predicate_1.parameters.push_back(parser::pddl::fromStringParam("r2d2", "robot"));
  predicate_1.parameters.push_back(parser::pddl::fromStringParam("bedroom", "room"));
  predicate_1.negate = false;

  cocg_ast::Node predicate_2;
  predicate_2.node_type = cocg_ast::Node::PREDICATE;
  predicate_2.node_id = 0;
  predicate_2.name = "robot_at";
  predicate_2.parameters.push_back(parser::pddl::fromStringParam("r2d2", "robot"));
  predicate_2.parameters.push_back(parser::pddl::fromStringParam("kitchen", "room"));
  predicate_2.negate = false;

  cocg_ast::Node predicate_3;
  predicate_3.node_type = cocg_ast::Node::PREDICATE;
  predicate_3.node_id = 0;
  predicate_3.name = "person_at";
  predicate_3.parameters.push_back(parser::pddl::fromStringParam("paco", "person"));
  predicate_3.parameters.push_back(parser::pddl::fromStringParam("bedroom", "room"));
  predicate_3.negate = false;

  cocg_ast::Node predicate_4;
  predicate_4.node_type = cocg_ast::Node::PREDICATE;
  predicate_4.node_id = 0;
  predicate_4.name = "person_at";
  predicate_4.parameters.push_back(parser::pddl::fromStringParam("paco", "person"));
  predicate_4.parameters.push_back(parser::pddl::fromStringParam("kitchen", "room"));
  predicate_4.negate = false;

  cocg_ast::Tree pn_1;
  pn_1.nodes.push_back(predicate_1);

  ASSERT_EQ(parser::pddl::toString(pn_1), "(robot_at r2d2 bedroom)");

  cocg_ast::Tree pn_2;
  pn_2.nodes.push_back(predicate_2);

  ASSERT_EQ(parser::pddl::toString(pn_2), "(robot_at r2d2 kitchen)");

  cocg_ast::Tree pn_3;
  pn_3.nodes.push_back(predicate_3);

  ASSERT_EQ(parser::pddl::toString(pn_3), "(person_at paco bedroom)");

  cocg_ast::Tree pn_4;
  pn_4.nodes.push_back(predicate_4);

  ASSERT_EQ(parser::pddl::toString(pn_4), "(person_at paco kitchen)");

  std::vector<cocg_ast::Tree> not_subtrees;
  not_subtrees.push_back(pn_2);
  auto not_tree = parser::pddl::fromSubtrees(not_subtrees, cocg_ast::Node::NOT);

  ASSERT_EQ(parser::pddl::toString(*not_tree), "(not (robot_at r2d2 kitchen))");

  std::vector<cocg_ast::Tree> not_subtrees_2;
  not_subtrees_2.push_back(*not_tree);
  auto not_tree_2 = parser::pddl::fromSubtrees(not_subtrees_2, cocg_ast::Node::NOT);

  ASSERT_EQ(parser::pddl::toString(*not_tree_2), "(robot_at r2d2 kitchen)");

  std::vector<cocg_ast::Tree> or_subtrees;
  or_subtrees.push_back(pn_3);
  or_subtrees.push_back(pn_4);
  auto or_tree = parser::pddl::fromSubtrees(or_subtrees, cocg_ast::Node::OR);

  ASSERT_EQ(
    parser::pddl::toString(
      *or_tree), "(or (person_at paco bedroom)(person_at paco kitchen))");

  std::vector<cocg_ast::Tree> not_subtrees_3;
  not_subtrees_3.push_back(*or_tree);
  auto not_tree_3 = parser::pddl::fromSubtrees(not_subtrees_3, cocg_ast::Node::NOT);

  ASSERT_EQ(
    parser::pddl::toString(*not_tree_3),
    "(and (not (person_at paco bedroom))(not (person_at paco kitchen)))");

  std::vector<cocg_ast::Tree> and_subtrees;
  and_subtrees.push_back(pn_1);
  and_subtrees.push_back(*not_tree_2);
  and_subtrees.push_back(*not_tree_3);
  auto and_tree = parser::pddl::fromSubtrees(and_subtrees, cocg_ast::Node::AND);

  ASSERT_EQ(
    parser::pddl::toString(*and_tree), std::string("(and (robot_at r2d2 bedroom)") +
    std::string("(robot_at r2d2 kitchen)(and (not (person_at paco ") +
    std::string("bedroom))(not (person_at paco kitchen))))"));
}

TEST(domain_types, predicate_tree_from_string)
{
  std::string expresion = std::string("(and (robot_at r2d2 bedroom)(not ") +
    std::string("(robot_at r2d2 kitchen))(or (person_at paco bedroom)(person_at paco kitchen)))");

  cocg_ast::Tree tree;
  parser::pddl::fromString(tree, expresion);

  cocg_ast::Node and_node = tree.nodes[0];
  ASSERT_EQ(and_node.node_type, cocg_ast::Node::AND);
  ASSERT_FALSE(and_node.negate);

  cocg_ast::Node p1_node = tree.nodes[1];
  ASSERT_EQ(p1_node.node_type, cocg_ast::Node::PREDICATE);
  ASSERT_EQ(p1_node.name, "robot_at");
  ASSERT_EQ(p1_node.parameters[0].name, "r2d2");
  ASSERT_EQ(p1_node.parameters[1].name, "bedroom");
  ASSERT_FALSE(p1_node.negate);

  cocg_ast::Node pnot_node = tree.nodes[2];
  ASSERT_EQ(pnot_node.node_type, cocg_ast::Node::NOT);
  ASSERT_FALSE(pnot_node.negate);

  cocg_ast::Node p2_node = tree.nodes[3];
  ASSERT_EQ(p2_node.node_type, cocg_ast::Node::PREDICATE);
  ASSERT_EQ(p2_node.name, "robot_at");
  ASSERT_EQ(p2_node.parameters[0].name, "r2d2");
  ASSERT_EQ(p2_node.parameters[1].name, "kitchen");
  ASSERT_TRUE(p2_node.negate);

  ASSERT_EQ(parser::pddl::toString(tree), expresion);

  std::string expresion2 = std::string("(and (person_at ?0 ?2)(not (person_at ?0 ?1)))");
  cocg_ast::Tree tree2;
  parser::pddl::fromString(tree2, expresion2);

  ASSERT_EQ(parser::pddl::toString(tree2), expresion2);
}

TEST(domain_types, predicate_tree_from_string_2)
{
  std::string expresion = std::string("(not (and (robot_at r2d2 bedroom)") +
    std::string("(robot_at r2d2 kitchen)))");

  cocg_ast::Tree tree;
  parser::pddl::fromString(tree, expresion);

  cocg_ast::Node not_node = tree.nodes[0];
  ASSERT_EQ(not_node.node_type, cocg_ast::Node::NOT);
  ASSERT_FALSE(not_node.negate);

  cocg_ast::Node and_node = tree.nodes[1];
  ASSERT_EQ(and_node.node_type, cocg_ast::Node::AND);
  ASSERT_TRUE(and_node.negate);

  cocg_ast::Node p1_node = tree.nodes[2];
  ASSERT_EQ(p1_node.node_type, cocg_ast::Node::PREDICATE);
  ASSERT_EQ(p1_node.name, "robot_at");
  ASSERT_EQ(p1_node.parameters[0].name, "r2d2");
  ASSERT_EQ(p1_node.parameters[1].name, "bedroom");
  ASSERT_TRUE(and_node.negate);

  cocg_ast::Node p2_node = tree.nodes[3];
  ASSERT_EQ(p2_node.node_type, cocg_ast::Node::PREDICATE);
  ASSERT_EQ(p2_node.name, "robot_at");
  ASSERT_EQ(p2_node.parameters[0].name, "r2d2");
  ASSERT_EQ(p2_node.parameters[1].name, "kitchen");
  ASSERT_TRUE(p2_node.negate);

  std::string expresion_eq = std::string("(or (not (robot_at r2d2 bedroom))") +
    std::string("(not (robot_at r2d2 kitchen)))");

  ASSERT_EQ(parser::pddl::toString(tree), expresion_eq);

  std::string expresion2 = std::string("(and (person_at ?0 ?2)(not (person_at ?0 ?1)))");
  cocg_ast::Tree tree2;
  parser::pddl::fromString(tree2, expresion2);

  ASSERT_EQ(parser::pddl::toString(tree2), expresion2);
}

TEST(domain_types, predicate_tree_from_string_3)
{
  std::string expresion = std::string("(and (patrolled ro1) (patrolled ro2) (patrolled ro3))");
  cocg_ast::Tree tree;
  parser::pddl::fromString(tree, expresion);

  ASSERT_EQ(parser::pddl::toString(tree), "(and (patrolled ro1)(patrolled ro2)(patrolled ro3))");
}

TEST(domain_types, predicate_tree_from_string_4)
{
  std::string expresion = std::string("  (  and (patrolled ro1) (patrolled ro2) (patrolled ro3))");
  cocg_ast::Tree tree;
  parser::pddl::fromString(tree, expresion);

  ASSERT_EQ(parser::pddl::toString(tree), "(and (patrolled ro1)(patrolled ro2)(patrolled ro3))");
}

TEST(domain_types, predicate_tree_from_string_negative)
{
  std::string expresion = std::string("(and (robot_at r2d2 bedroom)(not ") +
    std::string("(robot_at r2d2 kitchen))(or (person_at paco bedroom)(person_at paco kitchen)))");

  cocg_ast::Tree tree;
  parser::pddl::fromString(tree, expresion);

  ASSERT_EQ(parser::pddl::toString(tree), expresion);

  std::string expresion2 = std::string("(and (person_at ?0 ?2)(not (person_at ?0 ?1)))");
  cocg_ast::Tree tree2;
  parser::pddl::fromString(tree2, expresion2);

  ASSERT_EQ(parser::pddl::toString(tree2), expresion2);
}

TEST(domain_types, get_predicates)
{
  std::string expresion_1 = std::string("(and (robot_at r2d2 bedroom)(not ") +
    std::string("(robot_at r2d2 kitchen))(or (person_at paco bedroom)(person_at paco kitchen)))");

  cocg_ast::Tree tree_1;
  parser::pddl::fromString(tree_1, expresion_1);

  std::vector<cocg_ast::Node> predicates_1;
  parser::pddl::getPredicates(predicates_1, tree_1);

  ASSERT_EQ(4u, predicates_1.size());
  ASSERT_EQ(predicates_1[0].name, "robot_at");
  ASSERT_EQ(predicates_1[0].parameters[0].name, "r2d2");
  ASSERT_EQ(predicates_1[0].parameters[1].name, "bedroom");
  ASSERT_FALSE(predicates_1[0].negate);

  ASSERT_EQ(predicates_1[1].name, "robot_at");
  ASSERT_EQ(predicates_1[1].parameters[0].name, "r2d2");
  ASSERT_EQ(predicates_1[1].parameters[1].name, "kitchen");
  ASSERT_TRUE(predicates_1[1].negate);

  ASSERT_EQ(predicates_1[2].name, "person_at");
  ASSERT_EQ(predicates_1[2].parameters[0].name, "paco");
  ASSERT_EQ(predicates_1[2].parameters[1].name, "bedroom");
  ASSERT_FALSE(predicates_1[2].negate);

  ASSERT_EQ(predicates_1[3].name, "person_at");
  ASSERT_EQ(predicates_1[3].parameters[0].name, "paco");
  ASSERT_EQ(predicates_1[3].parameters[1].name, "kitchen");
  ASSERT_FALSE(predicates_1[3].negate);
}

int main(int argc, char ** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

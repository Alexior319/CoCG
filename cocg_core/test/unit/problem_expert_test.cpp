
#include <memory>
#include <string>
#include <vector>
#include <filesystem>

#include "cocg_ast/node.h"
#include "cocg_ast/param.h"
#include "cocg_ast/tree.h"
#include "cocg_core/cocg_domain_expert.h"
#include "cocg_core/cocg_problem_expert.h"
#include "gtest/gtest.h"

class problem_expert : public ::testing::Test {
 protected:
  static void SetUpTestCase() {}
};

TEST(problem_expert, addget_instances) {
  std::string pkgpath =
      std::filesystem::current_path().string();
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_simple_pe.pddl");
  std::string domain_str((std::istreambuf_iterator<char>(domain_ifs)),
                         std::istreambuf_iterator<char>());

  auto domain_expert = std::make_shared<cocg::DomainExpert>(domain_str);
  cocg::ProblemExpert problem_expert(domain_expert);

  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("Paco", "person")));
  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("Paco", "person")));
  ASSERT_FALSE(problem_expert.addInstance(
      parser::pddl::fromStringParam("Paco", "room")));
  ASSERT_FALSE(problem_expert.addInstance(
      parser::pddl::fromStringParam("Paco", "SCIENTIFIC")));
  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("r2d2", "robot")));

  ASSERT_EQ(problem_expert.getInstances().size(), 2);
  ASSERT_EQ(problem_expert.getInstances()[0].name, "Paco");
  ASSERT_EQ(problem_expert.getInstances()[0].type, "person");
  ASSERT_EQ(problem_expert.getInstances()[1].name, "r2d2");
  ASSERT_EQ(problem_expert.getInstances()[1].type, "robot");

  ASSERT_TRUE(problem_expert.removeInstance(
      parser::pddl::fromStringParam("Paco", "person")));
  ASSERT_EQ(problem_expert.getInstances().size(), 1);
  ASSERT_EQ(problem_expert.getInstances()[0].name, "r2d2");
  ASSERT_EQ(problem_expert.getInstances()[0].type, "robot");

  auto paco_instance = problem_expert.getInstance("Paco");
  ASSERT_FALSE(paco_instance);
  auto r2d2_instance = problem_expert.getInstance("r2d2");
  ASSERT_TRUE(r2d2_instance);
  ASSERT_EQ(r2d2_instance.value().name, "r2d2");
  ASSERT_EQ(r2d2_instance.value().type, "robot");
}

TEST(problem_expert, add_functions) {
  std::string pkgpath =
      std::filesystem::current_path().string();
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_simple_pe.pddl");
  std::string domain_str((std::istreambuf_iterator<char>(domain_ifs)),
                         std::istreambuf_iterator<char>());

  auto domain_expert = std::make_shared<cocg::DomainExpert>(domain_str);
  cocg::ProblemExpert problem_expert(domain_expert);

  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("bedroom", "room")));
  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("kitchen", "room_with_teleporter")));

  cocg_ast::Node function_1;
  function_1.node_type = cocg_ast::Node::FUNCTION;
  function_1.name = "room_distance";
  function_1.parameters.push_back(
      parser::pddl::fromStringParam("bedroom", "room"));
  function_1.parameters.push_back(
      parser::pddl::fromStringParam("kitchen", "room_with_teleporter"));
  function_1.value = 1.23;

  ASSERT_EQ(function_1.name, "room_distance");
  ASSERT_EQ(function_1.parameters.size(), 2);
  ASSERT_EQ(function_1.parameters[0].name, "bedroom");
  ASSERT_EQ(function_1.parameters[0].type, "room");
  ASSERT_EQ(function_1.parameters[1].name, "kitchen");
  ASSERT_EQ(function_1.parameters[1].type, "room_with_teleporter");
  ASSERT_EQ(function_1.value, 1.23);

  ASSERT_TRUE(problem_expert.addFunction(function_1));

  ASSERT_EQ(
      problem_expert.getProblem(),
      std::string("( define ( problem problem_1 )\n( :domain simple )\n") +
          std::string("( :objects\n\tbedroom - room\n\tkitchen - "
                      "room_with_teleporter\n)\n") +
          std::string("( :init\n\t( = ( room_distance bedroom kitchen ) "
                      "1.2300000000 )\n)\n") +
          std::string("( :goal\n\t\t)\n)\n"));

  cocg_ast::Node function_2;
  function_2.node_type = cocg_ast::Node::FUNCTION;
  function_2.name = "room_distance";
  function_2.parameters.push_back(
      parser::pddl::fromStringParam("kitchen", "room_with_teleporter"));
  function_2.parameters.push_back(
      parser::pddl::fromStringParam("bedroom", "room"));
  function_2.value = 2.34;

  ASSERT_EQ(function_2.name, "room_distance");
  ASSERT_EQ(function_2.parameters.size(), 2);
  ASSERT_EQ(function_2.parameters[0].name, "kitchen");
  ASSERT_EQ(function_2.parameters[0].type, "room_with_teleporter");
  ASSERT_EQ(function_2.parameters[1].name, "bedroom");
  ASSERT_EQ(function_2.parameters[1].type, "room");
  ASSERT_EQ(function_2.value, 2.34);

  ASSERT_TRUE(problem_expert.addFunction(function_2));

  ASSERT_EQ(
      problem_expert.getProblem(),
      std::string("( define ( problem problem_1 )\n( :domain simple )\n") +
          std::string("( :objects\n\tbedroom - room\n\tkitchen - "
                      "room_with_teleporter\n)\n") +
          std::string("( :init\n\t( = ( room_distance bedroom kitchen ) "
                      "1.2300000000 )\n") +
          std::string(
              "\t( = ( room_distance kitchen bedroom ) 2.3400000000 )\n)\n") +
          std::string("( :goal\n\t\t)\n)\n"));

  function_2.value = 3.45;

  ASSERT_TRUE(problem_expert.addFunction(function_2));

  ASSERT_EQ(
      problem_expert.getProblem(),
      std::string("( define ( problem problem_1 )\n( :domain simple )\n") +
          std::string("( :objects\n\tbedroom - room\n\tkitchen - "
                      "room_with_teleporter\n)\n") +
          std::string("( :init\n\t( = ( room_distance bedroom kitchen ) "
                      "1.2300000000 )\n") +
          std::string(
              "\t( = ( room_distance kitchen bedroom ) 3.4500000000 )\n)\n") +
          std::string("( :goal\n\t\t)\n)\n"));

  cocg_ast::Node function_3;
  function_3.node_type = cocg_ast::Node::FUNCTION;
  function_3.name = "room_temperature";
  function_3.parameters.push_back(
      parser::pddl::fromStringParam("bedroom", "room"));
  function_3.parameters.push_back(
      parser::pddl::fromStringParam("kitchen", "room_with_teleporter"));
  function_3.value = 2.34;

  ASSERT_FALSE(problem_expert.addFunction(function_3));
  ASSERT_FALSE(problem_expert.removeFunction(function_3));

  ASSERT_TRUE(problem_expert.removeInstance(
      parser::pddl::fromStringParam("kitchen", "room_with_teleporter")));
}

TEST(problem_expert, addget_predicates) {
  std::string pkgpath =
      std::filesystem::current_path().string();
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_simple_pe.pddl");
  std::string domain_str((std::istreambuf_iterator<char>(domain_ifs)),
                         std::istreambuf_iterator<char>());

  auto domain_expert = std::make_shared<cocg::DomainExpert>(domain_str);
  cocg::ProblemExpert problem_expert(domain_expert);

  cocg_ast::Node predicate_1;
  predicate_1.node_type = cocg_ast::Node::PREDICATE;
  predicate_1.name = "robot_at";
  predicate_1.parameters.push_back(
      parser::pddl::fromStringParam("r2d2", "robot"));
  predicate_1.parameters.push_back(
      parser::pddl::fromStringParam("bedroom", "room"));

  ASSERT_EQ(predicate_1.name, "robot_at");
  ASSERT_EQ(predicate_1.parameters.size(), 2);
  ASSERT_EQ(predicate_1.parameters[0].name, "r2d2");
  ASSERT_EQ(predicate_1.parameters[0].type, "robot");
  ASSERT_EQ(predicate_1.parameters[1].name, "bedroom");
  ASSERT_EQ(predicate_1.parameters[1].type, "room");

  cocg_ast::Node predicate_2;
  predicate_2.node_type = cocg_ast::Node::PREDICATE;
  predicate_2.name = "robot_at";
  predicate_2.parameters.push_back(
      parser::pddl::fromStringParam("r2d2", "robot"));
  predicate_2.parameters.push_back(
      parser::pddl::fromStringParam("kitchen", "room"));

  cocg_ast::Node predicate_3;
  predicate_3.node_type = cocg_ast::Node::PREDICATE;
  predicate_3.name = "person_at";
  predicate_3.parameters.push_back(
      parser::pddl::fromStringParam("paco", "person"));
  predicate_3.parameters.push_back(
      parser::pddl::fromStringParam("bedroom", "room"));

  cocg_ast::Node predicate_4;
  predicate_4.node_type = cocg_ast::Node::PREDICATE;
  predicate_4.name = "person_at";
  predicate_4.parameters.push_back(
      parser::pddl::fromStringParam("paco", "person"));
  predicate_4.parameters.push_back(
      parser::pddl::fromStringParam("kitchen", "room"));

  ASSERT_EQ(predicate_4.name, "person_at");
  ASSERT_EQ(predicate_4.parameters.size(), 2);
  ASSERT_EQ(predicate_4.parameters[0].name, "paco");
  ASSERT_EQ(predicate_4.parameters[0].type, "person");
  ASSERT_EQ(predicate_4.parameters[1].name, "kitchen");
  ASSERT_EQ(predicate_4.parameters[1].type, "room");

  cocg_ast::Node predicate_5;
  predicate_5.node_type = cocg_ast::Node::PREDICATE;
  predicate_5.name = "person_at";
  predicate_5.parameters.push_back(
      parser::pddl::fromStringParam("paco", "person"));
  predicate_5.parameters.push_back(
      parser::pddl::fromStringParam("kitchen", "room"));
  predicate_5.parameters.push_back(
      parser::pddl::fromStringParam("r2d2", "robot"));
  predicate_5.parameters.push_back(
      parser::pddl::fromStringParam("bedroom", "room"));

  cocg_ast::Node predicate_6;
  predicate_6.node_type = cocg_ast::Node::PREDICATE;
  predicate_6.name = "person_at";
  predicate_6.parameters.push_back(
      parser::pddl::fromStringParam("kitchen", "room"));
  predicate_6.parameters.push_back(
      parser::pddl::fromStringParam("paco", "person"));

  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("paco", "person")));
  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("r2d2", "robot")));
  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("bedroom", "room")));
  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("kitchen", "room")));

  std::vector<cocg::Predicate> predicates = problem_expert.getPredicates();
  ASSERT_TRUE(predicates.empty());

  ASSERT_TRUE(problem_expert.addPredicate(predicate_1));
  predicates = problem_expert.getPredicates();
  ASSERT_FALSE(predicates.empty());
  ASSERT_TRUE(problem_expert.addPredicate(predicate_1));
  ASSERT_TRUE(problem_expert.addPredicate(predicate_2));
  ASSERT_TRUE(problem_expert.addPredicate(predicate_3));
  ASSERT_TRUE(problem_expert.addPredicate(predicate_4));
  ASSERT_FALSE(problem_expert.addPredicate(predicate_5));
  ASSERT_FALSE(problem_expert.addPredicate(predicate_6));

  predicates = problem_expert.getPredicates();
  ASSERT_EQ(predicates.size(), 4);

  auto pred_2 = problem_expert.getPredicate("(robot_at r2d2 kitchen)");
  ASSERT_TRUE(pred_2);
  ASSERT_EQ(pred_2.value().name, "robot_at");
  ASSERT_EQ(pred_2.value().parameters.size(), 2);
  ASSERT_EQ(pred_2.value().parameters[0].name, "r2d2");
  ASSERT_EQ(pred_2.value().parameters[0].type, "robot");
  ASSERT_EQ(pred_2.value().parameters[1].name, "kitchen");
  ASSERT_EQ(pred_2.value().parameters[1].type, "room");

  ASSERT_FALSE(problem_expert.removePredicate(predicate_5));
  ASSERT_TRUE(problem_expert.removePredicate(predicate_4));
  ASSERT_TRUE(problem_expert.removePredicate(predicate_4));

  predicates = problem_expert.getPredicates();
  ASSERT_EQ(predicates.size(), 3);

  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("bathroom", "room_with_teleporter")));

  cocg_ast::Node predicate_7;
  predicate_7.node_type = cocg_ast::Node::PREDICATE;
  predicate_7.name = "is_teleporter_enabled";
  predicate_7.parameters.push_back(
      parser::pddl::fromStringParam("bathroom", "room_with_teleporter"));

  ASSERT_EQ(predicate_7.name, "is_teleporter_enabled");
  ASSERT_EQ(predicate_7.parameters.size(), 1);
  ASSERT_EQ(predicate_7.parameters[0].name, "bathroom");
  ASSERT_EQ(predicate_7.parameters[0].type, "room_with_teleporter");

  ASSERT_TRUE(problem_expert.addPredicate(predicate_7));

  cocg_ast::Node predicate_8;
  predicate_8.node_type = cocg_ast::Node::PREDICATE;
  predicate_8.name = "is_teleporter_destination";
  predicate_8.parameters.push_back(
      parser::pddl::fromStringParam("bathroom", "room_with_teleporter"));

  ASSERT_EQ(predicate_8.name, "is_teleporter_destination");
  ASSERT_EQ(predicate_8.parameters.size(), 1);
  ASSERT_EQ(predicate_8.parameters[0].name, "bathroom");
  ASSERT_EQ(predicate_8.parameters[0].type, "room_with_teleporter");

  ASSERT_TRUE(problem_expert.addPredicate(predicate_8));

  ASSERT_TRUE(problem_expert.removeInstance(
      parser::pddl::fromStringParam("bathroom", "room_with_teleporter")));
}

TEST(problem_expert, addget_unknown_predicates) {
  std::string pkgpath =
      std::filesystem::current_path().string();
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_simple_pe.pddl");
  std::string domain_str((std::istreambuf_iterator<char>(domain_ifs)),
                         std::istreambuf_iterator<char>());

  auto domain_expert = std::make_shared<cocg::DomainExpert>(domain_str);
  cocg::ProblemExpert problem_expert(domain_expert);

  cocg_ast::Node on_table_b3;
  on_table_b3.node_type = cocg_ast::Node::PREDICATE;
  on_table_b3.name = "robot_at";
  on_table_b3.parameters.push_back(
      parser::pddl::fromStringParam("r2d2", "robot"));
  on_table_b3.parameters.push_back(
      parser::pddl::fromStringParam("bedroom", "room"));

  ASSERT_EQ(on_table_b3.name, "robot_at");
  ASSERT_EQ(on_table_b3.parameters.size(), 2);
  ASSERT_EQ(on_table_b3.parameters[0].name, "r2d2");
  ASSERT_EQ(on_table_b3.parameters[0].type, "robot");
  ASSERT_EQ(on_table_b3.parameters[1].name, "bedroom");
  ASSERT_EQ(on_table_b3.parameters[1].type, "room");

  cocg_ast::Node predicate_3;
  predicate_3.node_type = cocg_ast::Node::PREDICATE;
  predicate_3.name = "person_at";
  predicate_3.parameters.push_back(
      parser::pddl::fromStringParam("paco", "person"));
  predicate_3.parameters.push_back(
      parser::pddl::fromStringParam("bedroom", "room"));

  auto unknown_1 =
      parser::pddl::fromString("(unknown (robot_at r2d2 kitchen))");
  auto oneof_1 = parser::pddl::fromString(
      "(oneof (person_at paco kitchen) (person_at paco bedroom))");
  auto or_1 = parser::pddl::fromString(
      "(or (person_at paco kitchen) (person_at paco bedroom))");

  cocg_ast::Node predicate_5_invalid;
  predicate_5_invalid.node_type = cocg_ast::Node::PREDICATE;
  predicate_5_invalid.name = "person_at";
  predicate_5_invalid.parameters.push_back(
      parser::pddl::fromStringParam("paco", "person"));
  predicate_5_invalid.parameters.push_back(
      parser::pddl::fromStringParam("kitchen", "room"));
  predicate_5_invalid.parameters.push_back(
      parser::pddl::fromStringParam("r2d2", "robot"));
  predicate_5_invalid.parameters.push_back(
      parser::pddl::fromStringParam("bedroom", "room"));

  cocg_ast::Node predicate_6_invalid;
  predicate_6_invalid.node_type = cocg_ast::Node::PREDICATE;
  predicate_6_invalid.name = "person_at";
  predicate_6_invalid.parameters.push_back(
      parser::pddl::fromStringParam("kitchen", "room"));
  predicate_6_invalid.parameters.push_back(
      parser::pddl::fromStringParam("paco", "person"));

  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("paco", "person")));
  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("r2d2", "robot")));
  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("bedroom", "room")));
  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("kitchen", "room")));

  std::vector<cocg::Predicate> predicates = problem_expert.getPredicates();
  ASSERT_TRUE(predicates.empty());

  ASSERT_TRUE(problem_expert.addPredicate(on_table_b3));
  ASSERT_TRUE(problem_expert.addPredicate(on_table_b3));
  ASSERT_TRUE(problem_expert.addPredicate(predicate_3));
  ASSERT_TRUE(problem_expert.addConditional(unknown_1));
  ASSERT_TRUE(problem_expert.addConditional(oneof_1));
  ASSERT_TRUE(problem_expert.addConditional(oneof_1));
  ASSERT_TRUE(problem_expert.addConditional(or_1));

  predicates = problem_expert.getPredicates();
  auto conditionals = problem_expert.getConditionals();
  ASSERT_EQ(predicates.size(), 2);
  ASSERT_EQ(conditionals.size(), 3);

  ASSERT_TRUE(problem_expert.removeConditional(unknown_1));
  ASSERT_TRUE(problem_expert.removeConditional(oneof_1));
  ASSERT_TRUE(problem_expert.removeConditional(oneof_1));
  ASSERT_TRUE(problem_expert.removeConditional(or_1));
}

TEST(problem_expert, addget_functions) {
  std::string pkgpath =
      std::filesystem::current_path().string();
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_charging.pddl");
  std::string domain_str((std::istreambuf_iterator<char>(domain_ifs)),
                         std::istreambuf_iterator<char>());

  auto domain_expert = std::make_shared<cocg::DomainExpert>(domain_str);
  cocg::ProblemExpert problem_expert(domain_expert);

  cocg_ast::Node function_1;
  function_1.node_type = cocg_ast::Node::FUNCTION;
  function_1.name = "speed";
  function_1.parameters.push_back(
      parser::pddl::fromStringParam("r2d2", "robot"));
  function_1.value = 3;

  ASSERT_EQ(function_1.name, "speed");
  ASSERT_EQ(function_1.parameters.size(), 1);
  ASSERT_EQ(function_1.parameters[0].name, "r2d2");
  ASSERT_EQ(function_1.parameters[0].type, "robot");
  ASSERT_EQ(function_1.value, 3);

  cocg_ast::Node function_2;
  function_2.node_type = cocg_ast::Node::FUNCTION;
  function_2.name = "distance";
  function_2.parameters.push_back(
      parser::pddl::fromStringParam("wp1", "waypoint"));
  function_2.parameters.push_back(
      parser::pddl::fromStringParam("wp2", "waypoint"));
  function_2.value = 15;

  ASSERT_EQ(function_2.name, "distance");
  ASSERT_EQ(function_2.parameters.size(), 2);
  ASSERT_EQ(function_2.parameters[0].name, "wp1");
  ASSERT_EQ(function_2.parameters[0].type, "waypoint");
  ASSERT_EQ(function_2.parameters[1].name, "wp2");
  ASSERT_EQ(function_2.parameters[1].type, "waypoint");
  ASSERT_EQ(function_2.value, 15);

  cocg_ast::Node function_3;
  function_3.node_type = cocg_ast::Node::FUNCTION;
  function_3.name = "speed";
  function_3.parameters.push_back(
      parser::pddl::fromStringParam("r2d2", "robot"));
  function_3.parameters.push_back(
      parser::pddl::fromStringParam("wp1", "waypoint"));

  cocg_ast::Node function_4;
  function_4.node_type = cocg_ast::Node::FUNCTION;
  function_4.name = "distance";
  function_4.parameters.push_back(
      parser::pddl::fromStringParam("r2d2", "robot"));
  function_4.parameters.push_back(
      parser::pddl::fromStringParam("wp1", "waypoint"));

  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("r2d2", "robot")));
  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("wp1", "waypoint")));
  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("wp2", "waypoint")));

  std::vector<cocg::Function> functions = problem_expert.getFunctions();
  ASSERT_TRUE(functions.empty());

  ASSERT_TRUE(problem_expert.addFunction(function_1));
  functions = problem_expert.getFunctions();
  ASSERT_FALSE(functions.empty());
  ASSERT_TRUE(problem_expert.addFunction(function_1));
  ASSERT_TRUE(problem_expert.addFunction(function_2));
  ASSERT_FALSE(problem_expert.addFunction(function_3));
  ASSERT_FALSE(problem_expert.addFunction(function_4));

  functions = problem_expert.getFunctions();
  ASSERT_EQ(functions.size(), 2);

  auto func_2 = problem_expert.getFunction("(distance wp1 wp2)");
  ASSERT_TRUE(func_2);
  ASSERT_EQ(func_2.value().name, "distance");
  ASSERT_EQ(func_2.value().parameters.size(), 2);
  ASSERT_EQ(func_2.value().parameters[0].name, "wp1");
  ASSERT_EQ(func_2.value().parameters[0].type, "waypoint");
  ASSERT_EQ(func_2.value().parameters[1].name, "wp2");
  ASSERT_EQ(func_2.value().parameters[1].type, "waypoint");
  ASSERT_EQ(func_2.value().value, 15);

  ASSERT_FALSE(problem_expert.removeFunction(function_3));
  ASSERT_TRUE(problem_expert.removeFunction(function_2));

  functions = problem_expert.getFunctions();
  ASSERT_EQ(functions.size(), 1);
}

TEST(problem_expert, addget_goals) {
  std::string pkgpath =
      std::filesystem::current_path().string();
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_simple_pe.pddl");
  std::string domain_str((std::istreambuf_iterator<char>(domain_ifs)),
                         std::istreambuf_iterator<char>());

  auto domain_expert = std::make_shared<cocg::DomainExpert>(domain_str);
  cocg::ProblemExpert problem_expert(domain_expert);

  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("paco", "person")));
  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("r2d2", "robot")));
  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("bedroom", "room")));
  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("kitchen", "room")));

  cocg_ast::Tree goal;
  parser::pddl::fromString(
      goal, "(and (robot_at r2d2 bedroom)(person_at paco kitchen))");
  ASSERT_EQ(parser::pddl::toString(goal),
            "(and (robot_at r2d2 bedroom)(person_at paco kitchen))");

  cocg_ast::Tree goal2;
  parser::pddl::fromString(
      goal2, "(and (robot_at r2d2 bedroom)(not(person_at paco kitchen)))");
  ASSERT_EQ(parser::pddl::toString(goal2),
            "(and (robot_at r2d2 bedroom)(not (person_at paco kitchen)))");

  ASSERT_TRUE(problem_expert.setGoal(goal));
  ASSERT_TRUE(problem_expert.setGoal(goal2));

  ASSERT_EQ(parser::pddl::toString(problem_expert.getGoal()),
            "(and (robot_at r2d2 bedroom)(not (person_at paco kitchen)))");

  const cocg_ast::Tree& goal3 = problem_expert.getGoal();
  ASSERT_EQ(parser::pddl::toString(goal3),
            "(and (robot_at r2d2 bedroom)(not (person_at paco kitchen)))");

  ASSERT_TRUE(problem_expert.clearGoal());
  ASSERT_TRUE(problem_expert.clearGoal());

  ASSERT_EQ(parser::pddl::toString(problem_expert.getGoal()), "");

  cocg_ast::Tree goal4;
  parser::pddl::fromString(goal4,
                           "(and (or (robot_at r2d2 bedroom) (robot_at r2d2 "
                           "kitchen)) (not (person_at paco kitchen)))");
  ASSERT_TRUE(problem_expert.setGoal(goal4));
}

TEST(problem_expert, empty_goals) {
  std::string pkgpath =
      std::filesystem::current_path().string();
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_simple_pe.pddl");
  std::string domain_str((std::istreambuf_iterator<char>(domain_ifs)),
                         std::istreambuf_iterator<char>());

  auto domain_expert = std::make_shared<cocg::DomainExpert>(domain_str);
  cocg::ProblemExpert problem_expert(domain_expert);

  cocg_ast::Tree goal;
  ASSERT_FALSE(problem_expert.isValidGoal(goal));
}

TEST(problem_expert, get_problem) {
  std::string pkgpath =
      std::filesystem::current_path().string();
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_simple_pe.pddl");
  std::string domain_str((std::istreambuf_iterator<char>(domain_ifs)),
                         std::istreambuf_iterator<char>());

  auto domain_expert = std::make_shared<cocg::DomainExpert>(domain_str);
  cocg::ProblemExpert problem_expert(domain_expert);

  cocg_ast::Node predicate_1;
  predicate_1.node_type = cocg_ast::Node::PREDICATE;
  predicate_1.name = "robot_at";
  predicate_1.parameters.push_back(
      parser::pddl::fromStringParam("r2d2", "robot"));
  predicate_1.parameters.push_back(
      parser::pddl::fromStringParam("bedroom", "room"));

  cocg_ast::Node predicate_2;
  predicate_2.node_type = cocg_ast::Node::PREDICATE;
  predicate_2.name = "robot_at";
  predicate_2.parameters.push_back(
      parser::pddl::fromStringParam("r2d2", "robot"));
  predicate_2.parameters.push_back(
      parser::pddl::fromStringParam("kitchen", "room"));

  cocg_ast::Node predicate_3;
  predicate_3.node_type = cocg_ast::Node::PREDICATE;
  predicate_3.name = "person_at";
  predicate_3.parameters.push_back(
      parser::pddl::fromStringParam("paco", "person"));
  predicate_3.parameters.push_back(
      parser::pddl::fromStringParam("bedroom", "room"));

  cocg_ast::Node predicate_4;
  predicate_4.node_type = cocg_ast::Node::PREDICATE;
  predicate_4.name = "person_at";
  predicate_4.parameters.push_back(
      parser::pddl::fromStringParam("paco", "person"));
  predicate_4.parameters.push_back(
      parser::pddl::fromStringParam("kitchen", "room"));

  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("paco", "person")));
  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("r2d2", "robot")));
  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("bedroom", "room")));
  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("kitchen", "room")));

  ASSERT_TRUE(problem_expert.addPredicate(predicate_1));
  ASSERT_TRUE(problem_expert.addPredicate(predicate_2));
  ASSERT_TRUE(problem_expert.addPredicate(predicate_3));
  ASSERT_TRUE(problem_expert.addPredicate(predicate_4));

  cocg_ast::Tree goal;
  parser::pddl::fromString(
      goal, "(and (robot_at r2d2 bedroom)(person_at paco kitchen))");
  ASSERT_TRUE(problem_expert.setGoal(goal));

  ASSERT_EQ(problem_expert.getProblem(),
            std::string("( define ( problem problem_1 )\n( :domain simple ") +
                std::string(")\n( :objects\n\tpaco - person\n\tr2d2 - "
                            "robot\n\tbedroom kitchen - room\n)\n") +
                std::string("( :init\n\t( robot_at r2d2 bedroom )\n\t( "
                            "robot_at r2d2 kitchen )\n\t( ") +
                std::string("person_at paco bedroom )\n\t( person_at paco "
                            "kitchen )\n)\n( :goal\n\t( ") +
                std::string("and\n\t\t( robot_at r2d2 bedroom )\n\t\t( "
                            "person_at paco kitchen )\n\t)\n)\n)\n"));

  ASSERT_TRUE(problem_expert.clearKnowledge());
  ASSERT_EQ(problem_expert.getPredicates().size(), 0);
  ASSERT_EQ(problem_expert.getFunctions().size(), 0);
  ASSERT_EQ(problem_expert.getInstances().size(), 0);
}

TEST(problem_expert, get_problem_observe) {
  std::string pkgpath =
      std::filesystem::current_path().string();
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_blocks_observe.pddl");
  std::string domain_str((std::istreambuf_iterator<char>(domain_ifs)),
                         std::istreambuf_iterator<char>());

  auto domain_expert = std::make_shared<cocg::DomainExpert>(domain_str);
  cocg::ProblemExpert problem_expert(domain_expert);

  cocg_ast::Node on_table_b1;
  on_table_b1.node_type = cocg_ast::Node::PREDICATE;
  on_table_b1.name = "ontable";
  on_table_b1.parameters.push_back(
      parser::pddl::fromStringParam("b1", "block"));

  cocg_ast::Node clear_b1;
  clear_b1.node_type = cocg_ast::Node::PREDICATE;
  clear_b1.name = "clear";
  clear_b1.parameters.push_back(parser::pddl::fromStringParam("b1", "block"));

  cocg_ast::Node on_table_b3;
  on_table_b3.node_type = cocg_ast::Node::PREDICATE;
  on_table_b3.name = "ontable";
  on_table_b3.parameters.push_back(
      parser::pddl::fromStringParam("b3", "block"));

  cocg_ast::Node clear_b2;
  clear_b2.node_type = cocg_ast::Node::PREDICATE;
  clear_b2.name = "clear";
  clear_b2.parameters.push_back(parser::pddl::fromStringParam("b2", "block"));

  ASSERT_TRUE(
      problem_expert.addInstance(parser::pddl::fromStringParam("b1", "block")));
  ASSERT_TRUE(
      problem_expert.addInstance(parser::pddl::fromStringParam("b2", "block")));
  ASSERT_TRUE(
      problem_expert.addInstance(parser::pddl::fromStringParam("b3", "block")));

  auto unknown_cond_1 = cocg::Unknown("(unknown (ontable b2))");
  auto unknown_cond_2 = cocg::Unknown("(unknown (on b2 b3))");
  ASSERT_TRUE(problem_expert.addConditional(unknown_cond_1));
  ASSERT_TRUE(problem_expert.addConditional(unknown_cond_2));

  auto oneof_cond = cocg::OneOf("(oneof (ontable b2) (on b2 b3))");
  ASSERT_TRUE(problem_expert.addConditional(oneof_cond));

  auto or_cond = cocg::Or("(or (on b2 b3) (on b3 b2))");
  ASSERT_TRUE(problem_expert.addConditional(or_cond));
  ASSERT_TRUE(problem_expert.removeConditional(or_cond));
  ASSERT_TRUE(problem_expert.addConditional(or_cond));

  auto or_cond2 = cocg::Or("(or (not (clear b1)) (clear b1))");
  ASSERT_TRUE(problem_expert.addConditional(or_cond2));
  ASSERT_TRUE(problem_expert.removeConditional(or_cond2));
  ASSERT_TRUE(problem_expert.addConditional(or_cond2));
  ASSERT_TRUE(problem_expert.addConditional(or_cond2));
  ASSERT_TRUE(problem_expert.addPredicate(on_table_b1));
  ASSERT_TRUE(problem_expert.addPredicate(clear_b1));
  ASSERT_TRUE(problem_expert.addPredicate(on_table_b3));
  ASSERT_TRUE(problem_expert.addPredicate(clear_b2));
  ASSERT_TRUE(problem_expert.removePredicate(clear_b2));

  cocg_ast::Tree goal;
  parser::pddl::fromString(goal, "(and (on b1 b2))");
  ASSERT_TRUE(problem_expert.setGoal(goal));

  auto tmp = std::string(
                 "( define ( problem problem_1 )\n( :domain blocksworld )\n( "
                 ":objects\n\tb1 b2 b3 ") +
             std::string(
                 "- block\n)\n( :init\n\t( ontable b1 )\n\t( clear b1 )\n\t( "
                 "ontable b3 )\n\t( unknown ( ontable ") +
             std::string(
                 "b2 )\t)\n\t( unknown ( on b2 b3 )\t)\n\t( oneof\n\t\t( "
                 "ontable b2 )\n\t\t( on b2 b3 )\n\t)\n\t( or") +
             std::string(
                 "\n\t\t( on b2 b3 )\n\t\t( on b3 b2 )\n\t)\n\t( or\n\t\t( not "
                 "( clear b1 ) )\n\t\t( clear b1 )\n\t)\n)\n( :goal\n\t\t\t( "
                 "on b1 b2 )\n\t)\n)\n");
  auto problem_str = problem_expert.getProblem();
  std::cout << problem_str;
  ASSERT_EQ(problem_str, tmp);

  ASSERT_TRUE(problem_expert.clearKnowledge());
  ASSERT_EQ(problem_expert.getPredicates().size(), 0);
  ASSERT_EQ(problem_expert.getFunctions().size(), 0);
  ASSERT_EQ(problem_expert.getInstances().size(), 0);
  ASSERT_EQ(problem_expert.getConditionals().size(), 0);
}

TEST(problem_expert, add_problem) {
  std::string pkgpath =
      std::filesystem::current_path().string();
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_simple_pe.pddl");
  std::string domain_str((std::istreambuf_iterator<char>(domain_ifs)),
                         std::istreambuf_iterator<char>());

  auto domain_expert = std::make_shared<cocg::DomainExpert>(domain_str);
  cocg::ProblemExpert problem_expert(domain_expert);

  ASSERT_FALSE(problem_expert.addProblem(""));

  // Empty domain name
  std::ifstream problem_empty_domain_ifs(pkgpath +
                                         "/pddl/problem_empty_domain.pddl");
  std::string problem_empty_domain_str(
      (std::istreambuf_iterator<char>(problem_empty_domain_ifs)),
      std::istreambuf_iterator<char>());
  ASSERT_FALSE(problem_expert.addProblem(problem_empty_domain_str));

  // Domain doesn't exist
  std::ifstream problem_charging_ifs(pkgpath + "/pddl/problem_charging.pddl");
  std::string problem_charging_str(
      (std::istreambuf_iterator<char>(problem_charging_ifs)),
      std::istreambuf_iterator<char>());
  ASSERT_FALSE(problem_expert.addProblem(problem_charging_str));

  // missing syntax causes std::runtime_error
  std::ifstream problem_unexpected_syntax_ifs(
      pkgpath + "/pddl/problem_unexpected_syntax.pddl");
  std::string problem_unexpected_syntax_str(
      (std::istreambuf_iterator<char>(problem_unexpected_syntax_ifs)),
      std::istreambuf_iterator<char>());
  ASSERT_FALSE(problem_expert.addProblem(problem_unexpected_syntax_str));

  std::ifstream problem_ifs(pkgpath + "/pddl/problem_simple_1_pe.pddl");
  std::string problem_str((std::istreambuf_iterator<char>(problem_ifs)),
                          std::istreambuf_iterator<char>());
  ASSERT_TRUE(problem_expert.addProblem(problem_str));

  ASSERT_TRUE(problem_expert.isValidType("robot"));
  ASSERT_TRUE(problem_expert.isValidType("person"));
  ASSERT_TRUE(problem_expert.isValidType("room"));
  ASSERT_TRUE(problem_expert.isValidType("room_with_teleporter"));
  ASSERT_TRUE(problem_expert.isValidType("message"));

  ASSERT_EQ(problem_expert.getInstances().size(), 5);
  ASSERT_EQ(problem_expert.getPredicates().size(), 2);
  ASSERT_EQ(problem_expert.getFunctions().size(), 1);

  ASSERT_TRUE(problem_expert.existInstance("leia"));
  ASSERT_TRUE(problem_expert.existInstance("jack"));
  ASSERT_TRUE(problem_expert.existInstance("kitchen"));
  ASSERT_TRUE(problem_expert.existInstance("bedroom"));
  ASSERT_TRUE(problem_expert.existInstance("m1"));

  ASSERT_FALSE(problem_expert.existInstance("r2d2"));
  ASSERT_FALSE(problem_expert.existInstance("hallway"));
  ASSERT_FALSE(problem_expert.existInstance("m2"));

  ASSERT_TRUE(problem_expert.existPredicate(
      parser::pddl::fromStringPredicate("(robot_at leia kitchen)")));
  ASSERT_TRUE(problem_expert.existPredicate(
      parser::pddl::fromStringPredicate("(person_at jack bedroom)")));

  ASSERT_EQ(parser::pddl::toString(problem_expert.getGoal()),
            "(and (robot_talk leia m1 jack))");

  ASSERT_EQ(
      problem_expert.getProblem(),
      std::string("( define ( problem problem_1 )\n( :domain simple )\n( :objects\n\tjack - person\n\tm1 - message\n\tleia - robot\n\tkitchen bedroom - room\n)\n( :init\n\t( robot_at leia kitchen )\n\t( person_at jack bedroom )\n\t( = ( room_distance kitchen bedroom ) 10.0000000000 )\n)\n( :goal\n\t\t\t( robot_talk leia m1 jack )\n\t)\n)\n"));

  ASSERT_TRUE(problem_expert.clearKnowledge());
  ASSERT_EQ(problem_expert.getPredicates().size(), 0);
  ASSERT_EQ(problem_expert.getFunctions().size(), 0);
  ASSERT_EQ(problem_expert.getInstances().size(), 0);
}

TEST(problem_expert, add_problem_with_constants) {
  std::string pkgpath =
      std::filesystem::current_path().string();
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_simple_constants.pddl");
  std::string domain_str((std::istreambuf_iterator<char>(domain_ifs)),
                         std::istreambuf_iterator<char>());

  auto domain_expert = std::make_shared<cocg::DomainExpert>(domain_str);
  cocg::ProblemExpert problem_expert(domain_expert);

  std::ifstream problem_1_ifs(pkgpath +
                              "/pddl/problem_simple_constants_1.pddl");
  std::string problem_1_str((std::istreambuf_iterator<char>(problem_1_ifs)),
                            std::istreambuf_iterator<char>());
  ASSERT_TRUE(problem_expert.addProblem(problem_1_str));

  ASSERT_TRUE(problem_expert.isValidType("robot"));
  ASSERT_TRUE(problem_expert.isValidType("person"));
  ASSERT_TRUE(problem_expert.isValidType("room"));
  ASSERT_TRUE(problem_expert.isValidType("teleporter_room"));
  ASSERT_TRUE(problem_expert.isValidType("message"));

  ASSERT_EQ(problem_expert.getInstances().size(), 7);
  ASSERT_EQ(problem_expert.getPredicates().size(), 2);
  ASSERT_EQ(problem_expert.getFunctions().size(), 0);

  ASSERT_TRUE(problem_expert.existInstance("leia"));
  ASSERT_TRUE(problem_expert.existInstance("lema"));
  ASSERT_TRUE(problem_expert.existInstance("jack"));
  ASSERT_TRUE(problem_expert.existInstance("john"));
  ASSERT_TRUE(problem_expert.existInstance("kitchen"));
  ASSERT_TRUE(problem_expert.existInstance("bedroom"));
  ASSERT_TRUE(problem_expert.existInstance("m1"));

  ASSERT_FALSE(problem_expert.existInstance("r2d2"));
  ASSERT_FALSE(problem_expert.existInstance("hallway"));
  ASSERT_FALSE(problem_expert.existInstance("m2"));

  ASSERT_TRUE(problem_expert.existPredicate(
      parser::pddl::fromStringPredicate("(robot_at leia kitchen)")));
  ASSERT_TRUE(problem_expert.existPredicate(
      parser::pddl::fromStringPredicate("(person_at jack bedroom)")));

  ASSERT_EQ(parser::pddl::toString(problem_expert.getGoal()),
            "(and (robot_talk leia m1 jack))");

  ASSERT_EQ(
      problem_expert.getProblem(),
      std::string("( define ( problem problem_1 )\n( :domain simple )\n( :objects\n\tm1 - message\n\tkitchen bedroom - room\n)\n( :init\n\t( robot_at leia kitchen )\n\t( person_at jack bedroom )\n)\n( :goal\n\t\t\t( robot_talk leia m1 jack )\n\t)\n)\n"));

  ASSERT_TRUE(problem_expert.clearKnowledge());
  ASSERT_EQ(problem_expert.getPredicates().size(), 0);
  ASSERT_EQ(problem_expert.getFunctions().size(), 0);
  ASSERT_EQ(problem_expert.getInstances().size(), 0);
  ASSERT_EQ(
      problem_expert.getProblem(),
      std::string("( define ( problem problem_1 )\n( :domain simple )\n") +
          std::string("( :objects\n)\n( :init\n)\n( :goal\n\t\t)\n)\n"));

  std::ifstream problem_2_ifs(pkgpath +
                              "/pddl/problem_simple_constants_2.pddl");
  std::string problem_2_str((std::istreambuf_iterator<char>(problem_2_ifs)),
                            std::istreambuf_iterator<char>());
  ASSERT_TRUE(problem_expert.addProblem(problem_2_str));

  ASSERT_NE(problem_1_str, problem_2_str);
  ASSERT_NE(problem_expert.getProblem(), problem_2_str);
  ASSERT_EQ(problem_expert.getProblem(), std::string("( define ( problem problem_1 )\n( :domain simple )\n( :objects\n\tm1 - message\n\tkitchen bedroom - room\n)\n( :init\n\t( robot_at leia kitchen )\n\t( person_at jack bedroom )\n)\n( :goal\n\t\t\t( robot_talk leia m1 jack )\n\t)\n)\n"));
}

TEST(problem_expert, add_problem_observe) {
  std::string pkgpath =
      std::filesystem::current_path().string();
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_blocks_observe.pddl");
  std::string domain_str((std::istreambuf_iterator<char>(domain_ifs)),
                         std::istreambuf_iterator<char>());

  auto domain_expert = std::make_shared<cocg::DomainExpert>(domain_str);
  cocg::ProblemExpert problem_expert(domain_expert);

  std::ifstream problem_1_ifs(pkgpath + "/pddl/problem_blocks_observe.pddl");
  std::string problem_1_str((std::istreambuf_iterator<char>(problem_1_ifs)),
                            std::istreambuf_iterator<char>());
  ASSERT_TRUE(problem_expert.addProblem(problem_1_str));

  ASSERT_TRUE(problem_expert.isValidType("block"));

  ASSERT_EQ(problem_expert.getInstances().size(), 3);
  ASSERT_EQ(problem_expert.getPredicates().size(), 2);
  ASSERT_EQ(problem_expert.getFunctions().size(), 0);
  ASSERT_EQ(problem_expert.getConditionals().size(), 14);

  ASSERT_TRUE(problem_expert.existInstance("b1"));
  ASSERT_TRUE(problem_expert.existInstance("b2"));
  ASSERT_TRUE(problem_expert.existInstance("b3"));

  ASSERT_TRUE(problem_expert.existPredicate(
      parser::pddl::fromStringPredicate("(ontable b1)")));
  ASSERT_TRUE(problem_expert.existPredicate(
      parser::pddl::fromStringPredicate("(clear b1)")));

  ASSERT_EQ(parser::pddl::toString(problem_expert.getGoal()),
            "(and (on b2 b1)(on b3 b2))");

  auto tmp1 = std::string(
                  "( define ( problem problem_1 )\n( :domain blocksworld )\n( "
                  ":objects\n\tb1 b2 b3 - block\n)") +
              std::string(
                  "\n( :init\n\t( ontable b1 )\n\t( clear b1 )\n\t( unknown ( "
                  "ontable b3 )\t)\n\t( unknown ( clear b3 )") +
              std::string(
                  "\t)\n\t( unknown ( on b3 b2 )\t)\n\t( unknown ( ontable b2 "
                  ")\t)\n\t( unknown ( clear b2 )\t)\n\t( ") +
              std::string(
                  "unknown ( on b2 b3 )\t)\n\t( or\n\t\t( not ( on b3 b2 ) "
                  ")\n\t\t( not ( on b2 b3 ) )\n\t)\n\t( ") +
              std::string(
                  "or\n\t\t( not ( on b2 b3 ) )\n\t\t( not ( on b3 b2 ) "
                  ")\n\t)\n\t( oneof\n\t\t( clear b3 )") +
              std::string(
                  "\n\t\t( clear b2 )\n\t)\n\t( oneof\n\t\t( ontable b3 "
                  ")\n\t\t( ontable b2 )\n\t)\n\t( oneof") +
              std::string(
                  "\n\t\t( ontable b3 )\n\t\t( on b3 b2 )\n\t)\n\t( "
                  "oneof\n\t\t( ontable b2 )\n\t\t( on b2 b3 ") +
              std::string(
                  ")\n\t)\n\t( oneof\n\t\t( clear b3 )\n\t\t( on b2 b3 "
                  ")\n\t)\n\t( oneof\n\t\t( clear b2 )\n") +
              std::string(
                  "\t\t( on b3 b2 )\n\t)\n)\n( :goal\n\t( and\n\t\t( on b2 b1 "
                  ")\n\t\t( on b3 b2 )\n\t)\n)\n)\n");
  auto tmp2 = problem_expert.getProblem();

  ASSERT_EQ(tmp1, tmp2);
  ASSERT_TRUE(problem_expert.clearKnowledge());
  auto tmp3 = problem_expert.getProblem();
  ASSERT_EQ(std::string("( define ( problem problem_1 )\n( :domain blocksworld "
                        ")\n( :objects\n)") +
                std::string("\n( :init\n)\n( :goal\n\t\t)\n)\n"),
            tmp3);
}

TEST(problem_expert, is_goal_satisfied) {
  std::string pkgpath =
      std::filesystem::current_path().string();
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_simple_pe.pddl");
  std::string domain_str((std::istreambuf_iterator<char>(domain_ifs)),
                         std::istreambuf_iterator<char>());

  auto domain_expert = std::make_shared<cocg::DomainExpert>(domain_str);
  cocg::ProblemExpert problem_expert(domain_expert);

  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("leia", "robot")));
  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("Jack", "person")));
  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("kitchen", "room")));
  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("bedroom", "room")));
  ASSERT_TRUE(problem_expert.addInstance(
      parser::pddl::fromStringParam("m1", "message")));

  ASSERT_TRUE(problem_expert.addPredicate(
      parser::pddl::fromStringPredicate("(robot_at leia kitchen)")));
  ASSERT_TRUE(problem_expert.addPredicate(
      parser::pddl::fromStringPredicate("(person_at Jack bedroom)")));

  std::string expression = "(and (robot_talk leia m1 Jack))";
  cocg_ast::Tree goal;
  parser::pddl::fromString(goal, expression);

  ASSERT_EQ(parser::pddl::toString(goal), "(and (robot_talk leia m1 Jack))");
  ASSERT_TRUE(problem_expert.setGoal(goal));
  ASSERT_FALSE(problem_expert.isGoalSatisfied(goal));

  ASSERT_TRUE(problem_expert.addPredicate(
      parser::pddl::fromStringPredicate("(robot_talk leia m1 Jack)")));

  ASSERT_TRUE(problem_expert.isGoalSatisfied(goal));
}

// int main(int argc, char** argv) {
//   testing::InitGoogleTest(&argc, argv);
//   return RUN_ALL_TESTS();
// }

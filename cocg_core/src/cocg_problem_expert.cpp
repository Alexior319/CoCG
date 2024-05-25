
#include "cocg_core/cocg_problem_expert.h"

#include <algorithm>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include "cocg_core/cocg_problem_expert_utils.h"
#include "cocg_core/types.h"
#include "cocg_core/utils.h"
#include "cocg_pddl_parser/Domain.h"
#include "cocg_pddl_parser/Instance.h"

namespace cocg {

ProblemExpert::ProblemExpert(std::shared_ptr<DomainExpert>& domain_expert)
    : domain_expert_(domain_expert) {}

ProblemExpert::ProblemExpert(const ProblemExpert& problem_expert) {
  domain_expert_ = problem_expert.domain_expert_;
  instances_ = problem_expert.instances_;
  predicates_ = problem_expert.predicates_;
  functions_ = problem_expert.functions_;
  conditionals_ = problem_expert.conditionals_;
  goal_ = problem_expert.goal_;
}

ProblemExpert::ProblemExpert(
    const std::shared_ptr<ProblemExpert>& problem_expert) {
  domain_expert_ = problem_expert->domain_expert_;
  instances_ = problem_expert->instances_;
  predicates_ = problem_expert->predicates_;
  functions_ = problem_expert->functions_;
  conditionals_ = problem_expert->conditionals_;
  goal_ = problem_expert->goal_;
}

bool ProblemExpert::addInstance(const cocg::Instance& instance) {
  if (!isValidType(instance.type)) {
    return false;
  }

  std::optional<cocg::Instance> existing_instance = getInstance(instance.name);
  bool exist_instance = existing_instance.has_value();

  if (exist_instance && existing_instance.value().type != instance.type) {
    return false;
  }

  if (!exist_instance) {
    instances_.push_back(instance);
  }

  return true;
}

std::vector<cocg::Instance> ProblemExpert::getInstances() { return instances_; }

bool ProblemExpert::removeInstance(const cocg::Instance& instance) {
  bool found = false;
  int i = 0;

  while (!found && i < instances_.size()) {
    if (instances_[i].name == instance.name) {
      found = true;
      instances_.erase(instances_.begin() + i);
    }
    i++;
  }

  removeInvalidPredicates(predicates_, instance);
  removeInvalidFunctions(functions_, instance);
  removeInvalidGoals(instance);

  return found;
}

std::optional<cocg::Instance> ProblemExpert::getInstance(
    const std::string& instance_name) {
  cocg::Instance ret;

  bool found = false;
  int i = 0;
  while (i < instances_.size() && !found) {
    if (instances_[i].name == instance_name) {
      found = true;
      ret = instances_[i];
    }
    i++;
  }

  if (found) {
    return ret;
  } else {
    return {};
  }
}

std::vector<cocg::Predicate> ProblemExpert::getPredicates() {
  return predicates_;
}

bool ProblemExpert::addPredicate(const cocg::Predicate& predicate) {
  if (!existPredicate(predicate)) {
    if (isValidPredicate(predicate)) {
      predicates_.push_back(predicate);
      return true;
    } else {
      return false;
    }
  } else {
    return true;
  }
}

bool ProblemExpert::removePredicate(const cocg::Predicate& predicate) {
  bool found = false;
  int i = 0;

  if (!isValidPredicate(predicate)) {  // if predicate is not valid, error
    return false;
  }
  while (!found && i < predicates_.size()) {
    if (parser::pddl::checkNodeEquality(predicates_[i], predicate)) {
      found = true;
      predicates_.erase(predicates_.begin() + i);
    }
    i++;
  }

  return true;
}

std::optional<cocg::Predicate> ProblemExpert::getPredicate(
    const std::string& expr) {
  cocg::Predicate ret;
  cocg::Predicate pred = parser::pddl::fromStringPredicate(expr);

  bool found = false;
  size_t i = 0;
  while (i < predicates_.size() && !found) {
    if (parser::pddl::checkNodeEquality(predicates_[i], pred)) {
      found = true;
      ret = predicates_[i];
    }
    i++;
  }

  if (found) {
    return ret;
  } else {
    return {};
  }
}

std::vector<cocg_ast::Tree> ProblemExpert::getConditionals() {
  return conditionals_;
}

bool ProblemExpert::addConditional(const cocg_ast::Tree& condition) {
  if (!existConditional(condition)) {
    if (isValidCondition(condition)) {
      if (condition.nodes[0].node_type == cocg_ast::Node::ONE_OF &&
          condition.nodes[0].children.size() == 1) {
        addPredicate(condition.nodes[1]);
      } else {
        conditionals_.push_back(condition);
      }
      return true;
    } else {
      return false;
    }
  } else {
    return true;
  }
}

bool ProblemExpert::removeConditional(const cocg_ast::Tree& condition) {
  if (!isValidCondition(condition)) {  // if predicate is not valid, error
    return false;
  }
  auto it =
      std::find_if(conditionals_.begin(), conditionals_.end(),
                   [&condition](const auto& ele) {
                     return parser::pddl::checkTreeEquality(ele, condition);
                   });
  if (it != conditionals_.end()) {
    conditionals_.erase(it);

    // NOTICE: refactor to its own function
    std::vector<cocg_ast::Tree> conditionals_to_remove;
    std::vector<cocg_ast::Tree> conditionals_to_add;
    if (condition.nodes[0].node_type == cocg_ast::Node::UNKNOWN) {
      for (auto c : conditionals_) {
        if (c.nodes[0].node_type == cocg_ast::Node::ONE_OF) {
          cocg_ast::Tree new_one_of;
          new_one_of.nodes.push_back(c.nodes[0]);
          new_one_of.nodes[0].children.clear();
          int num_children = 0;
          for (auto child_ind : c.nodes[0].children) {
            if (!parser::pddl::checkNodeEquality(c.nodes[child_ind],
                                                 condition.nodes[1])) {
              new_one_of.nodes.push_back(c.nodes[child_ind]);
              new_one_of.nodes[0].children.push_back(num_children + 1);
              num_children++;
            }
          }
          conditionals_to_remove.push_back(c);
          if (num_children > 0) {
            conditionals_to_add.push_back(new_one_of);
          }
        }
      }
    }
    for (const auto& c : conditionals_to_remove) {
      removeConditional(c);
    }
    for (const auto& c : conditionals_to_add) {
      addConditional(c);
    }
  }
  return true;
}

// TODO, not support or and negetive precondtion currently
bool ProblemExpert::removeConditionalUnknown(const cocg_ast::Tree& condition,
                                             bool known_to_true) {
  if (!isValidCondition(condition)) {  // if predicate is not valid, error
    return false;
  }
  auto it =
      std::find_if(conditionals_.begin(), conditionals_.end(),
                   [&condition](const auto& ele) {
                     return parser::pddl::checkTreeEquality(ele, condition);
                   });
  if (it != conditionals_.end()) {
    conditionals_.erase(it);

#ifdef OUTPUT_DEBUG_INFO
    std::cout << "[ProblemExpert] Found the unknown predicate to remove: "
              << parser::pddl::toString(condition)
              << ", it is known to be: " << known_to_true << std::endl;
#endif

    std::vector<cocg_ast::Tree> conditionals_to_remove;
    std::vector<cocg_ast::Tree> conditionals_to_add;
    if (condition.nodes[0].node_type == cocg_ast::Node::UNKNOWN) {
      for (auto c : conditionals_) {
        if (c.nodes[0].node_type == cocg_ast::Node::ONE_OF) {
          bool exist_same_node = false;
          for (const auto& c_child_node_ind : c.nodes[0].children) {
            if (parser::pddl::checkNodeEquality(c.nodes[c_child_node_ind],
                                                condition.nodes[1])) {
              exist_same_node = true;
              break;
            }
          }
          // condition not child of the oneof node, do nothing
          if (!exist_same_node) continue;

          cocg_ast::Tree new_one_of;
          new_one_of.nodes.push_back(c.nodes[0]);
          new_one_of.nodes[0].children.clear();
          int num_children = 0;
          if (!known_to_true) {
            for (auto child_ind : c.nodes[0].children) {
              if (!parser::pddl::checkNodeEquality(c.nodes[child_ind],
                                                   condition.nodes[1])) {
                new_one_of.nodes.push_back(c.nodes[child_ind]);
                new_one_of.nodes[0].children.push_back(num_children + 1);
                num_children++;
              }
            }
            conditionals_to_remove.push_back(c);
            if (num_children > 0) {
              conditionals_to_add.push_back(new_one_of);
            }
          } else {  // known to be true, donnot contain other facts
            for (auto child_ind : c.nodes[0].children) {
              if (parser::pddl::checkNodeEquality(c.nodes[child_ind],
                                                  condition.nodes[1])) {
                new_one_of.nodes.push_back(c.nodes[child_ind]);
                new_one_of.nodes[0].children.push_back(num_children + 1);
                num_children++;
              }
            }
            conditionals_to_remove.push_back(c);
            if (num_children > 0) {
              conditionals_to_add.push_back(new_one_of);
            }
          }
        }
      }
#ifdef OUTPUT_DEBUG_INFO
      std::cout << "[ProblemExpert] conditionals should be removed: ";
      for (const auto& c : conditionals_to_remove) {
        std::cout << parser::pddl::toString(c) << " ";
      }
      std::cout << std::endl;
      std::cout << "[ProblemExpert] conditionals should be added: ";
      for (const auto& c : conditionals_to_add) {
        std::cout << parser::pddl::toString(c) << " ";
      }
      std::cout << std::endl;
#endif
      for (const auto& c : conditionals_to_remove) {
        removeConditional(c);
      }
      for (const auto& c : conditionals_to_add) {
        addConditional(c);
      }
#ifdef OUTPUT_DEBUG_INFO
      std::cout << "[ProblemExpert] Remained conditionals: ";
      for (const auto& c : conditionals_) {
        std::cout << parser::pddl::toString(c) << " ";
      }
      std::cout << std::endl;
      std::cout << "[ProblemExpert] Remained predicates: ";
      for (const auto& c : predicates_) {
        std::cout << parser::pddl::toString(c) << " ";
      }
      std::cout << std::endl;
#endif
    }
    return true;
  }
  std::cout << "Conditional not found: " << parser::pddl::toString(condition)
            << std::endl;
  return false;
}

bool ProblemExpert::existConditional(const cocg_ast::Tree& condition) {
  return std::find(conditionals_.begin(), conditionals_.end(), condition) !=
         conditionals_.end();
}

std::vector<cocg::Function> ProblemExpert::getFunctions() { return functions_; }

bool ProblemExpert::addFunction(const cocg::Function& function) {
  if (!existFunction(function)) {
    if (isValidFunction(function)) {
      functions_.push_back(function);
      return true;
    } else {
      return false;
    }
  } else {
    return updateFunction(function);
  }
}

bool ProblemExpert::removeFunction(const cocg::Function& function) {
  bool found = false;
  int i = 0;

  if (!isValidFunction(function)) {  // if function is not valid, error
    return false;
  }
  while (!found && i < functions_.size()) {
    if (parser::pddl::checkNodeEquality(functions_[i], function)) {
      found = true;
      functions_.erase(functions_.begin() + i);
    }
    i++;
  }

  return true;
}

bool ProblemExpert::updateFunction(const cocg::Function& function) {
  if (existFunction(function)) {
    if (isValidFunction(function)) {
      removeFunction(function);
      functions_.push_back(function);
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

std::optional<cocg::Function> ProblemExpert::getFunction(
    const std::string& expr) {
  cocg::Function ret;
  cocg::Function func = parser::pddl::fromStringFunction(expr);

  bool found = false;
  size_t i = 0;
  while (i < functions_.size() && !found) {
    if (parser::pddl::checkNodeEquality(functions_[i], func)) {
      found = true;
      ret = functions_[i];
    }
    i++;
  }

  if (found) {
    return ret;
  } else {
    return {};
  }
}

void ProblemExpert::removeInvalidPredicates(
    std::vector<cocg::Predicate>& predicates, const cocg::Instance& instance) {
  for (auto rit = predicates.rbegin(); rit != predicates.rend(); ++rit) {
    if (std::find_if(rit->parameters.begin(), rit->parameters.end(),
                     [&](const cocg_ast::Param& param) {
                       return param.name == instance.name;
                     }) != rit->parameters.end()) {
      predicates.erase(std::next(rit).base());
    }
  }
}

void ProblemExpert::removeInvalidFunctions(
    std::vector<cocg::Function>& functions, const cocg::Instance& instance) {
  for (auto rit = functions.rbegin(); rit != functions.rend(); ++rit) {
    if (std::find_if(rit->parameters.begin(), rit->parameters.end(),
                     [&](const cocg_ast::Param& param) {
                       return param.name == instance.name;
                     }) != rit->parameters.end()) {
      functions.erase(std::next(rit).base());
    }
  }
}

void ProblemExpert::removeInvalidGoals(const cocg::Instance& instance) {
  // Get subgoals.
  auto subgoals = parser::pddl::getSubtrees(goal_);

  // Check for subgoals before continuing.
  if (subgoals.empty()) {
    return;
  }

  // Remove invalid subgoals.
  for (auto rit = subgoals.rbegin(); rit != subgoals.rend(); ++rit) {
    // Get predicates.
    std::vector<cocg_ast::Node> predicates;
    parser::pddl::getPredicates(predicates, *rit);

    // Check predicates for removed instance.
    bool params_valid = true;
    for (const auto& predicate : predicates) {
      if (std::find_if(predicate.parameters.begin(), predicate.parameters.end(),
                       [&](const cocg_ast::Param& param) {
                         return param.name == instance.name;
                       }) != predicate.parameters.end()) {
        params_valid = false;
        break;
      }
    }

    // Remove invalid subgoal.
    if (!params_valid) {
      subgoals.erase(std::next(rit).base());
      continue;
    }

    // Get functions.
    std::vector<cocg_ast::Node> functions;
    parser::pddl::getFunctions(functions, *rit);

    // Check functions for removed instance.
    params_valid = true;
    for (const auto& function : functions) {
      if (std::find_if(function.parameters.begin(), function.parameters.end(),
                       [&](const cocg_ast::Param& param) {
                         return param.name == instance.name;
                       }) != function.parameters.end()) {
        params_valid = false;
        break;
      }
    }

    // Remove invalid subgoal.
    if (!params_valid) {
      subgoals.erase(std::next(rit).base());
    }
  }

  // Create a new goal from the remaining subgoals.
  auto tree = parser::pddl::fromSubtrees(subgoals, goal_.nodes[0].node_type);
  if (tree) {
    goal_ = cocg::Goal(*tree);
  } else {
    goal_.nodes.clear();
  }
}

cocg::Goal ProblemExpert::getGoal() { return goal_; }

bool ProblemExpert::setGoal(const cocg::Goal& goal) {
  if (isValidGoal(goal)) {
    goal_ = goal;
    return true;
  } else {
    return false;
  }
}

bool ProblemExpert::isGoalSatisfied(const cocg::Goal& goal) {
  return check(goal, predicates_, functions_);
}

bool ProblemExpert::clearGoal() {
  goal_.nodes.clear();
  return true;
}

bool ProblemExpert::clearKnowledge() {
  instances_.clear();
  predicates_.clear();
  functions_.clear();
  conditionals_.clear();
  clearGoal();

  return true;
}

bool ProblemExpert::isValidType(const std::string& type) {
  auto valid_types = domain_expert_->getTypes();
  auto it = std::find(valid_types.begin(), valid_types.end(), type);

  return it != valid_types.end();
}

bool ProblemExpert::existInstance(const std::string& name) {
  bool found = false;
  int i = 0;

  while (!found && i < instances_.size()) {
    if (instances_[i].name == name) {
      found = true;
    }
    i++;
  }

  return found;
}

bool ProblemExpert::existPredicate(const cocg::Predicate& predicate) {
  bool found = false;
  int i = 0;

  while (!found && i < predicates_.size()) {
    if (parser::pddl::checkNodeEquality(predicates_[i], predicate)) {
      found = true;
    }
    i++;
  }

  return found;
}

bool ProblemExpert::existFunction(const cocg::Function& function) {
  bool found = false;
  int i = 0;

  while (!found && i < functions_.size()) {
    if (parser::pddl::checkNodeEquality(functions_[i], function)) {
      found = true;
    }
    i++;
  }

  return found;
}

bool ProblemExpert::isValidPredicate(const cocg::Predicate& predicate) {
  bool valid = false;

  const std::optional<cocg::Predicate>& model_predicate =
      domain_expert_->getPredicate(predicate.name);
  if (model_predicate) {
    if (model_predicate.value().parameters.size() ==
        predicate.parameters.size()) {
      bool same_types = true;
      int i = 0;
      while (same_types && i < predicate.parameters.size()) {
        auto arg_type = getInstance(predicate.parameters[i].name);

        if (!arg_type.has_value()) {
          same_types = false;
        } else if (arg_type.value().type !=
                   model_predicate.value().parameters[i].type) {
          bool isSubtype = false;
          for (std::string subType :
               model_predicate.value().parameters[i].sub_types) {
            if (arg_type.value().type == subType) {
              isSubtype = true;
              break;
            }
          }
          if (!isSubtype) {
            same_types = false;
          }
        }
        i++;
      }
      valid = same_types;
    }
  }

  return valid;
}

bool ProblemExpert::isValidFunction(const cocg::Function& function) {
  bool valid = false;

  const std::optional<cocg::Function>& model_function =
      domain_expert_->getFunction(function.name);
  if (model_function) {
    if (model_function.value().parameters.size() ==
        function.parameters.size()) {
      bool same_types = true;
      int i = 0;
      while (same_types && i < function.parameters.size()) {
        auto arg_type = getInstance(function.parameters[i].name);

        if (!arg_type.has_value()) {
          same_types = false;
        } else if (arg_type.value().type !=
                   model_function.value().parameters[i].type) {
          bool isSubtype = false;
          for (std::string subType :
               model_function.value().parameters[i].sub_types) {
            if (arg_type.value().type == subType) {
              isSubtype = true;
              break;
            }
          }
          if (!isSubtype) {
            same_types = false;
          }
        }
        i++;
      }
      valid = same_types;
    }
  }

  return valid;
}

bool ProblemExpert::isValidGoal(const cocg::Goal& goal) {
  return checkPredicateTreeTypes(goal, domain_expert_);
}

bool ProblemExpert::isValidCondition(const cocg_ast::Tree& cond) {
  return checkPredicateTreeTypes(cond, domain_expert_);
}

bool ProblemExpert::checkPredicateTreeTypes(
    const cocg_ast::Tree& tree, std::shared_ptr<DomainExpert>& domain_expert,
    uint8_t node_id) {
  if (node_id >= tree.nodes.size()) {
    return false;
  }

  switch (tree.nodes[node_id].node_type) {
    case cocg_ast::Node::AND: {
      bool ret = true;

      for (auto& child_id : tree.nodes[node_id].children) {
        ret = ret && checkPredicateTreeTypes(tree, domain_expert, child_id);
      }
      return ret;
    }

    case cocg_ast::Node::OR: {
      bool ret = true;

      for (auto& child_id : tree.nodes[node_id].children) {
        ret = ret && checkPredicateTreeTypes(tree, domain_expert, child_id);
      }
      return ret;
    }

    case cocg_ast::Node::NOT: {
      return checkPredicateTreeTypes(tree, domain_expert,
                                     tree.nodes[node_id].children[0]);
    }

    case cocg_ast::Node::UNKNOWN: {
      return tree.nodes[node_id].children.size() == 1 &&
             checkPredicateTreeTypes(tree, domain_expert,
                                     tree.nodes[node_id].children[0]);
    }

    case cocg_ast::Node::ONE_OF: {
      bool ret = true;

      for (auto& child_id : tree.nodes[node_id].children) {
        ret = ret && checkPredicateTreeTypes(tree, domain_expert, child_id);
      }
      return ret;
    }

    case cocg_ast::Node::PREDICATE: {
      return isValidPredicate(tree.nodes[node_id]);
    }

    case cocg_ast::Node::FUNCTION: {
      return isValidFunction(tree.nodes[node_id]);
    }

    case cocg_ast::Node::EXPRESSION: {
      bool ret = true;

      for (auto& child_id : tree.nodes[node_id].children) {
        ret = ret && checkPredicateTreeTypes(tree, domain_expert, child_id);
      }
      return ret;
    }

    case cocg_ast::Node::FUNCTION_MODIFIER: {
      bool ret = true;

      for (auto& child_id : tree.nodes[node_id].children) {
        ret = ret && checkPredicateTreeTypes(tree, domain_expert, child_id);
      }
      return ret;
    }

    case cocg_ast::Node::NUMBER: {
      return true;
    }

    default:
      // LCOV_EXCL_START
      std::cerr << "checkPredicateTreeTypes: Error parsing expresion ["
                << parser::pddl::toString(tree, node_id) << "]" << std::endl;
      // LCOV_EXCL_STOP
  }

  return false;
}

std::string ProblemExpert::getProblem() {
  parser::pddl::Domain domain(domain_expert_->getDomain());
  parser::pddl::Instance problem(domain);

  problem.name = "problem_1";

  for (const auto& instance : instances_) {
    bool is_constant =
        domain.getType(instance.type)->parseConstant(instance.name).first;
    if (is_constant) {
      std::cout << "Skipping adding constant as an problem :object: "
                << instance.name << " " << instance.type << std::endl;
    } else {
      problem.addObject(instance.name, instance.type);
    }
  }

  for (cocg_ast::Node predicate : predicates_) {
    StringVec v;

    for (size_t i = 0; i < predicate.parameters.size(); i++) {
      v.push_back(predicate.parameters[i].name);
    }

    std::transform(predicate.name.begin(), predicate.name.end(),
                   predicate.name.begin(), ::tolower);

    problem.addInit(predicate.name, v);
  }

  for (auto cond : conditionals_) {
    auto root_node = cond.nodes[0];
    std::vector<cocg_ast::Node> predicates;
    parser::pddl::getPredicates(predicates, cond, 0);
    switch (root_node.node_type) {
      case cocg_ast::Node::UNKNOWN: {
        auto predicate = predicates[0];
        StringVec v;
        for (size_t i = 0; i < predicate.parameters.size(); i++) {
          v.push_back(predicate.parameters[i].name);
        }
        std::transform(predicate.name.begin(), predicate.name.end(),
                       predicate.name.begin(), ::tolower);
        problem.addInitUnknown(predicate.name, v);
        break;
      }
      case cocg_ast::Node::ONE_OF: {
        std::vector<StringVec> v_vecs;
        std::vector<std::string> names;
        for (cocg_ast::Node predicate : predicates) {
          StringVec v;
          for (size_t i = 0; i < predicate.parameters.size(); i++) {
            v.push_back(predicate.parameters[i].name);
          }
          v_vecs.push_back(v);

          std::transform(predicate.name.begin(), predicate.name.end(),
                         predicate.name.begin(), ::tolower);
          names.push_back(predicate.name);
        }
        problem.addInitOneOf(names, v_vecs);
        break;
      }
      case cocg_ast::Node::OR: {
        std::vector<StringVec> vecs;
        std::vector<std::string> names;
        std::vector<bool> negates;
        for (auto predicate : predicates) {
          StringVec v;
          for (size_t i = 0; i < predicate.parameters.size(); i++) {
            v.push_back(predicate.parameters[i].name);
          }
          std::transform(predicate.name.begin(), predicate.name.end(),
                         predicate.name.begin(), ::tolower);

          names.push_back(predicate.name);
          vecs.push_back(v);
          negates.push_back(predicate.negate);
        }
        problem.addInitOr({names[0], names[1]}, {vecs[0], vecs[1]},
                          {negates[0], negates[1]});
        break;
      }
      default: {
        std::cerr << "Failed to add conditional node " << std::endl;
        break;
      }
    }
  }

  for (cocg_ast::Node function : functions_) {
    StringVec v;

    for (size_t i = 0; i < function.parameters.size(); i++) {
      v.push_back(function.parameters[i].name);
    }

    std::transform(function.name.begin(), function.name.end(),
                   function.name.begin(), ::tolower);

    problem.addInit(function.name, function.value, v);
  }

  const std::string gs = parser::pddl::toString(goal_);
  problem.addGoal(gs);

  std::ostringstream stream;
  stream << problem;
  return stream.str();
}

bool ProblemExpert::addProblem(const std::string& problem_str) {
  if (problem_str.empty()) {
    std::cerr << "Empty problem." << std::endl;
    return false;
  }
  parser::pddl::Domain domain(domain_expert_->getDomain());

  std::string lc_problem = problem_str;
  std::transform(problem_str.begin(), problem_str.end(), lc_problem.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  lc_problem = remove_comments(lc_problem);

  std::cout << "Domain:\n" << domain << std::endl;
  std::cout << "Problem:\n" << lc_problem << std::endl;

  parser::pddl::Instance problem(domain);

  std::string domain_name = problem.getDomainName(lc_problem);
  if (domain_name.empty()) {
    std::cerr << "Domain name is empty" << std::endl;
    return false;
  } else if (!domain_expert_->existDomain(domain_name)) {
    std::cerr << "Domain name does not exist: " << domain_name << std::endl;
    return false;
  }

  domain.name = domain_name;
  try {
    problem.parse(lc_problem);
  } catch (std::runtime_error ex) {
    // all errors thrown by the Stringreader object extend std::runtime_error
    std::cerr << ex.what() << std::endl;
    return false;
  }

  std::cout << "Parsed problem: " << problem << std::endl;

  for (unsigned i = 0; i < domain.types.size(); ++i) {
    if (domain.types[i]->constants.size()) {
      for (unsigned j = 0; j < domain.types[i]->constants.size(); ++j) {
        cocg::Instance instance;
        instance.name = domain.types[i]->constants[j];
        instance.type = domain.types[i]->name;
        std::cout << "Adding constant: " << instance.name << " "
                  << instance.type << std::endl;
        addInstance(instance);
      }
    }
  }

  for (unsigned i = 0; i < domain.types.size(); ++i) {
    if (domain.types[i]->objects.size()) {
      for (unsigned j = 0; j < domain.types[i]->objects.size(); ++j) {
        cocg::Instance instance;
        instance.name = domain.types[i]->objects[j];
        instance.type = domain.types[i]->name;
        std::cout << "Adding instance: " << instance.name << " "
                  << instance.type << std::endl;
        addInstance(instance);
      }
    }
  }

  for (auto ground : problem.init) {
    cocg_ast::Tree tree;
    auto tree_node = ground->getTree(tree, domain);
    switch (tree_node->node_type) {
      case cocg_ast::Node::PREDICATE: {
        cocg::Predicate pred_node(*tree_node);
        std::cout << "Adding predicate: "
                  << parser::pddl::toString(tree, tree_node->node_id)
                  << std::endl;
        if (!addPredicate(pred_node)) {
          std::cerr << "Failed to add predicate: "
                    << parser::pddl::toString(tree, tree_node->node_id)
                    << std::endl;
        }
      } break;
      case cocg_ast::Node::FUNCTION: {
        cocg::Function func_node(*tree_node);
        std::cout << "Adding function: "
                  << parser::pddl::toString(tree, tree_node->node_id)
                  << std::endl;
        if (!addFunction(func_node)) {
          std::cerr << "Failed to add function: "
                    << parser::pddl::toString(tree, tree_node->node_id)
                    << std::endl;
        }
      } break;
      default:
        break;
    }
  }

  for (auto cond : problem.init_cond) {
    cocg_ast::Tree tree;
    auto tree_node = cond->getTree(tree, domain);
    switch (tree_node->node_type) {
      case cocg_ast::Node::UNKNOWN: {
        auto tmp(*tree_node);
        std::cout << "Adding unknown predicate: "
                  << parser::pddl::toString(tree, tree_node->node_id)
                  << std::endl;
        if (!addConditional(tree)) {
          std::cerr << "Failed to add unknown predicate: "
                    << parser::pddl::toString(tree, tree_node->node_id)
                    << std::endl;
        }
        break;
      }
      case cocg_ast::Node::ONE_OF: {
        auto tmp(*tree_node);
        std::cout << "Adding oneof: "
                  << parser::pddl::toString(tree, tree_node->node_id)
                  << std::endl;
        if (!addConditional(tree)) {
          std::cerr << "Failed to add oneof : "
                    << parser::pddl::toString(tree, tree_node->node_id)
                    << std::endl;
        }
        break;
      }
      case cocg_ast::Node::OR: {
        cocg_ast::Tree or_tree;
        auto or_node = cond->getTree(or_tree, domain);
        std::cout << "Adding or node: "
                  << parser::pddl::toString(or_tree, or_node->node_id)
                  << std::endl;
        if (!addConditional(tree)) {
          std::cerr << "Failed to add or : "
                    << parser::pddl::toString(or_tree, or_node->node_id)
                    << std::endl;
        }
        break;
      }
      default:
        break;
    }
  }
  cocg_ast::Tree goal;
  auto node = problem.goal->getTree(goal, domain);
  std::cout << "Adding Goal: " << parser::pddl::toString(goal) << std::endl;
  if (setGoal(goal)) {
    std::cout << "Goal insertion ok" << std::endl;
  } else {
    std::cout << "Goal insertion failed" << std::endl;
  }

  return true;
}

}  // namespace cocg

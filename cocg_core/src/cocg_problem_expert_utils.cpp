
#include "cocg_core/cocg_problem_expert_utils.h"

#include <map>
#include <memory>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "cocg_pddl_parser/Utils.h"

namespace cocg {

// returns a 3-tuple (success, truth_value, 0). success is a bool indicating
// whether or not the expression was evaluated correctly. truth_value indicates
// the result of the evaluation.
std::tuple<bool, bool, double> evaluate(
    const cocg_ast::Tree& tree,
    std::shared_ptr<cocg::ProblemExpert> problem_client,
    std::vector<cocg::Predicate>& predicates,
    std::vector<cocg::Function>& functions, bool apply, bool use_state,
    uint8_t node_id, bool negate) {
  if (tree.nodes.empty()) {  // No expression
    return std::make_tuple(true, true, 0);
  }

  switch (tree.nodes[node_id].node_type) {
    case cocg_ast::Node::AND: {
      bool success = true;
      bool truth_value = true;

      for (auto& child_id : tree.nodes[node_id].children) {
        std::tuple<bool, bool, double> result =
            evaluate(tree, problem_client, predicates, functions, apply,
                     use_state, child_id, negate);
        success = success && std::get<0>(result);
        truth_value = truth_value && std::get<1>(result);
      }
      return std::make_tuple(success, truth_value, 0);
    }

    case cocg_ast::Node::FOR_ALL: {
      // NOTICE: this only handles a single predicate and a single parameter
      bool success = true;
      bool truth_value = true;
      auto all_instances = problem_client->getInstances();
      auto params = tree.nodes[node_id].parameters;
      std::unordered_map<std::string, std::vector<std::string>> instance_map;
      if (params.size() > 1) {
        std::cerr << "for all does not currently support multiple parameters"
                  << std::endl;
        return std::make_tuple(false, false, 0);
      }
      for (auto& instance : all_instances) {
        instance_map[instance.type].push_back(instance.name);
      }

      std::vector<cocg_ast::Tree> for_all_predicates;
      auto child_ind = tree.nodes[node_id].children[0];
      auto universal_pred_name = tree.nodes[child_ind].name;
      for (const auto& instance_name : instance_map[params[0].type]) {
        cocg_ast::Tree tree_pred = tree;
        auto ind = child_ind;
        while (tree_pred.nodes[ind].node_type != cocg_ast::Node::PREDICATE) {
          ind = tree_pred.nodes[ind].children[0];
        }
        for (auto& param : tree_pred.nodes[ind].parameters) {
          if (param.type == params[0].type) {
            param.name = instance_name;
          }
        }
        for_all_predicates.push_back(tree_pred);
      }

      for (const auto& tree_pred : for_all_predicates) {
        std::tuple<bool, bool, double> result =
            evaluate(tree_pred, problem_client, predicates, functions, apply,
                     use_state, child_ind, negate);
        success = success && std::get<0>(result);
        truth_value = truth_value && std::get<1>(result);
      }

      return std::make_tuple(success, truth_value, 0);
    }

    case cocg_ast::Node::OR: {
      bool success = true;
      bool truth_value = false;

      for (auto& child_id : tree.nodes[node_id].children) {
        std::tuple<bool, bool, double> result =
            evaluate(tree, problem_client, predicates, functions, apply,
                     use_state, child_id, negate);
        success = success && std::get<0>(result);
        truth_value = truth_value || std::get<1>(result);
      }
      return std::make_tuple(success, truth_value, 0);
    }

    case cocg_ast::Node::NOT: {
      return evaluate(tree, problem_client, predicates, functions, apply,
                      use_state, tree.nodes[node_id].children[0], !negate);
    }

    case cocg_ast::Node::PREDICATE: {
      bool success = true;
      bool value = true;

      if (apply) {
        if (use_state) {
          auto it = std::find_if(
              predicates.begin(), predicates.end(),
              std::bind(&parser::pddl::checkNodeEquality, std::placeholders::_1,
                        tree.nodes[node_id]));
          if (negate) {
            if (it != predicates.end()) {
              predicates.erase(it);
            }
            value = false;
          } else {
            if (it == predicates.end()) {
              predicates.push_back(tree.nodes[node_id]);
            }
          }
        } else {
          if (negate) {
            success =
                success && problem_client->removePredicate(tree.nodes[node_id]);
            value = false;
          } else {
            success =
                success && problem_client->addPredicate(tree.nodes[node_id]);
          }
        }
      } else {
        // negate | exist | output
        //   F    |   F   |   F
        //   F    |   T   |   T
        //   T    |   F   |   T
        //   T    |   T   |   F
        if (use_state) {
          value =
              negate ^ (std::find_if(predicates.begin(), predicates.end(),
                                     std::bind(&parser::pddl::checkNodeEquality,
                                               std::placeholders::_1,
                                               tree.nodes[node_id])) !=
                        predicates.end());
        } else {
          value = negate ^ problem_client->existPredicate(tree.nodes[node_id]);
        }
      }

      return std::make_tuple(success, value, 0);
    }

    case cocg_ast::Node::FUNCTION: {
      bool success = true;
      double value = 0;

      if (use_state) {
        auto it =
            std::find_if(functions.begin(), functions.end(),
                         std::bind(&parser::pddl::checkNodeEquality,
                                   std::placeholders::_1, tree.nodes[node_id]));
        if (it != functions.end()) {
          value = it->value;
        } else {
          success = false;
        }
      } else {
        std::optional<cocg_ast::Node> func =
            problem_client->getFunction(parser::pddl::toString(tree, node_id));

        if (func.has_value()) {
          value = func.value().value;
        } else {
          success = false;
        }
      }

      return std::make_tuple(success, false, value);
    }

    case cocg_ast::Node::EXPRESSION: {
      std::tuple<bool, bool, double> left =
          evaluate(tree, problem_client, predicates, functions, apply,
                   use_state, tree.nodes[node_id].children[0], negate);
      std::tuple<bool, bool, double> right =
          evaluate(tree, problem_client, predicates, functions, apply,
                   use_state, tree.nodes[node_id].children[1], negate);

      if (!std::get<0>(left) || !std::get<0>(right)) {
        return std::make_tuple(false, false, 0);
      }

      switch (tree.nodes[node_id].expression_type) {
        case cocg_ast::Node::COMP_GE:
          if (std::get<2>(left) >= std::get<2>(right)) {
            return std::make_tuple(true, true, 0);
          } else {
            return std::make_tuple(true, false, 0);
          }
          break;
        case cocg_ast::Node::COMP_GT:
          if (std::get<2>(left) > std::get<2>(right)) {
            return std::make_tuple(true, true, 0);
          } else {
            return std::make_tuple(true, false, 0);
          }
          break;
        case cocg_ast::Node::COMP_LE:
          if (std::get<2>(left) <= std::get<2>(right)) {
            return std::make_tuple(true, true, 0);
          } else {
            return std::make_tuple(true, false, 0);
          }
          break;
        case cocg_ast::Node::COMP_LT:
          if (std::get<2>(left) < std::get<2>(right)) {
            return std::make_tuple(true, true, 0);
          } else {
            return std::make_tuple(true, false, 0);
          }
          break;
        case cocg_ast::Node::COMP_EQ:
          if (std::get<2>(left) == std::get<2>(right)) {
            return std::make_tuple(true, true, 0);
          } else {
            return std::make_tuple(true, false, 0);
          }
          break;

        case cocg_ast::Node::ARITH_MULT:
          return std::make_tuple(true, false,
                                 std::get<2>(left) * std::get<2>(right));
          break;
        case cocg_ast::Node::ARITH_DIV:
          if (std::abs(std::get<2>(right)) > 1e-5) {
            return std::make_tuple(true, false,
                                   std::get<2>(left) / std::get<2>(right));
          } else {
            // Division by zero not allowed.
            return std::make_tuple(false, false, 0);
          }
          break;
        case cocg_ast::Node::ARITH_ADD:
          return std::make_tuple(true, false,
                                 std::get<2>(left) + std::get<2>(right));
          break;
        case cocg_ast::Node::ARITH_SUB:
          return std::make_tuple(true, false,
                                 std::get<2>(left) - std::get<2>(right));
          break;
        default:
          break;
      }

      return std::make_tuple(false, false, 0);
    }

    case cocg_ast::Node::FUNCTION_MODIFIER: {
      std::tuple<bool, bool, double> left =
          evaluate(tree, problem_client, predicates, functions, apply,
                   use_state, tree.nodes[node_id].children[0], negate);
      std::tuple<bool, bool, double> right =
          evaluate(tree, problem_client, predicates, functions, apply,
                   use_state, tree.nodes[node_id].children[1], negate);

      if (!std::get<0>(left) || !std::get<0>(right)) {
        return std::make_tuple(false, false, 0);
      }

      bool success = true;
      double value = 0;

      switch (tree.nodes[node_id].modifier_type) {
        case cocg_ast::Node::ASSIGN:
          value = std::get<2>(right);
          break;
        case cocg_ast::Node::INCREASE:
          value = std::get<2>(left) + std::get<2>(right);
          break;
        case cocg_ast::Node::DECREASE:
          value = std::get<2>(left) - std::get<2>(right);
          break;
        case cocg_ast::Node::SCALE_UP:
          value = std::get<2>(left) * std::get<2>(right);
          break;
        case cocg_ast::Node::SCALE_DOWN:
          // Division by zero not allowed.
          if (std::abs(std::get<2>(right)) > 1e-5) {
            value = std::get<2>(left) / std::get<2>(right);
          } else {
            success = false;
          }
          break;
        default:
          success = false;
          break;
      }

      if (success && apply) {
        uint8_t left_id = tree.nodes[node_id].children[0];
        if (use_state) {
          auto it = std::find_if(
              functions.begin(), functions.end(),
              std::bind(&parser::pddl::checkNodeEquality, std::placeholders::_1,
                        tree.nodes[left_id]));
          if (it != functions.end()) {
            it->value = value;
          } else {
            success = false;
          }
        } else {
          std::stringstream ss;
          ss << "(= " << parser::pddl::toString(tree, left_id) << " " << value
             << ")";
          problem_client->updateFunction(
              parser::pddl::fromStringFunction(ss.str()));
        }
      }

      return std::make_tuple(success, false, value);
    }

    case cocg_ast::Node::NUMBER: {
      return std::make_tuple(true, true, tree.nodes[node_id].value);
    }

      //    case cocg_ast::Node::UNKNOWN: {
      //          return std::make_tuple(true, true, 0);
      //    }

    default:
      std::cerr << "evaluate: Error parsing expression ["
                << parser::pddl::toString(tree, node_id) << "]" << std::endl;
  }

  return std::make_tuple(false, false, 0);
}

std::tuple<bool, bool, double> evaluate(
    const cocg_ast::Tree& tree,
    std::shared_ptr<cocg::ProblemExpert> problem_client, bool apply,
    uint32_t node_id) {
  std::vector<cocg::Predicate> predicates;
  std::vector<cocg::Function> functions;
  return evaluate(tree, problem_client, predicates, functions, apply, false,
                  node_id);
}

std::tuple<bool, bool, double> evaluate(
    const cocg_ast::Tree& tree, std::vector<cocg::Predicate>& predicates,
    std::vector<cocg::Function>& functions, bool apply, uint32_t node_id) {
  auto problem_client = std::make_shared<cocg::ProblemExpert>();
  return evaluate(tree, problem_client, predicates, functions, apply, true,
                  node_id);
}

bool check(const cocg_ast::Tree& tree,
           std::shared_ptr<cocg::ProblemExpert> problem_client,
           uint32_t node_id) {
  std::tuple<bool, bool, double> ret =
      evaluate(tree, problem_client, false, node_id);

  return std::get<1>(ret);
}

bool check(const cocg_ast::Tree& tree, std::vector<cocg::Predicate>& predicates,
           std::vector<cocg::Function>& functions, uint32_t node_id) {
  std::tuple<bool, bool, double> ret =
      evaluate(tree, predicates, functions, false, node_id);

  return std::get<1>(ret);
}

bool apply(const cocg_ast::Tree& tree,
           std::shared_ptr<cocg::ProblemExpert> problem_client,
           uint32_t node_id) {
  std::tuple<bool, bool, double> ret =
      evaluate(tree, problem_client, true, node_id);

  return std::get<0>(ret);
}

bool apply(const cocg_ast::Tree& tree, std::vector<cocg::Predicate>& predicates,
           std::vector<cocg::Function>& functions, uint32_t node_id) {
  std::tuple<bool, bool, double> ret =
      evaluate(tree, predicates, functions, true, node_id);

  return std::get<0>(ret);
}

std::pair<std::string, int> parse_action(const std::string& input) {
  std::string action = parser::pddl::getReducedString(input);
  int time = -1;

  size_t delim = action.find(":");
  if (delim != std::string::npos) {
    time = std::stoi(action.substr(delim + 1, action.length() - delim - 1));
    action.erase(action.begin() + delim, action.end());
  }

  action.erase(0, 1);  // remove initial (
  action.pop_back();   // remove last )

  return std::make_pair(action, time);
}

std::string get_action_expression(const std::string& input) {
  auto action = parse_action(input);
  return action.first;
}

int get_action_time(const std::string& input) {
  auto action = parse_action(input);
  return action.second;
}

std::string get_action_name(const std::string& input) {
  auto expr = get_action_expression(input);
  size_t delim = expr.find(" ");
  return expr.substr(0, delim);
}

std::vector<std::string> get_action_params(const std::string& input) {
  std::vector<std::string> ret;

  auto expr = get_action_expression(input);

  size_t delim = expr.find(" ");
  if (delim != std::string::npos) {
    expr.erase(expr.begin(), expr.begin() + delim + 1);
  }

  size_t start = 0, end = 0;
  while (end != std::string::npos) {
    end = expr.find(" ", start);
    auto param = expr.substr(
        start, (end == std::string::npos) ? std::string::npos : end - start);
    ret.push_back(param);
    start = ((end > (std::string::npos - 1)) ? std::string::npos : end + 1);
  }

  return ret;
}

}  // namespace cocg

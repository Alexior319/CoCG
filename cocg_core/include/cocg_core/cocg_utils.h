#ifndef COCG_CORE_COCG_UTILS_H_
#define COCG_CORE_COCG_UTILS_H_

#include <memory>
#include <tuple>

#include "cocg_core/cocg_problem_expert.h"
#include "cocg_core/cocg_problem_expert_utils.h"
#include "cocg_core/cont_plan_node.h"

namespace cocg {

struct CoCGState {
  std::vector<cocg::Predicate> predicates;  // predicates in this state
  std::vector<cocg::Function>
      functions;  // functions in this state, not used now

  using SharedPtr = std::shared_ptr<CoCGState>();

  // comparison operators
  bool operator==(const CoCGState& other) const {
    if (this->predicates != other.predicates) {
      return false;
    }
    if (this->functions != other.functions) {
      return false;
    }
    return true;
  }
  bool operator!=(const CoCGState& other) const {
    return !this->operator==(other);
  }
};

/**
 * @brief apply the effects of an actuation action to a state
 * @param init_state the initial state
 * @param action the actuation action
 * @return the goal state
 */
CoCGState apply_actuation_action(const CoCGState& init_state,
                                 const cocg_ast::Action& action);

/**
 * @brief apply the effects of a sensing action to a state
 * @param init_state the initial state
 * @param action the sensing action
 * @param sensing_result the result of the sensing action, true or false
 * @return the goal state
 */
CoCGState apply_sensing_action(const CoCGState& init_state,
                               const cocg_ast::Action& action,
                               bool sensing_result);

/**
 * @brief split a grounded action into action name and action arguments
 * @param grounded_action the grounded action, format: (action_name, arg1, arg2,
 * ...)
 * @param action_name the action name
 * @param action_args the action arguments
 */
void split_grounded_action(const std::string& grounded_action,
                           std::string& action_name,
                           std::vector<std::string>& action_args);

/**
 * @brief convert a plan node to an AST action
 * @param node the plan node in a contingent plan tree
 * @param domain_expert the domain expert that initiaized by a domain file
 * @return the AST action
 */
cocg_ast::Action::SharedPtr convert_plan_node_to_ast(
    const cocg::ContPlanNode::SharedPtr node,
    cocg::DomainExpert& domain_expert);

/**
 * @brief traverse a contingent planning tree until the next sensing node or the
 * nullptr
 * @param init_state the initial state
 * @param root the root of the contingent planning tree
 * @return a tuple with the goal state, the vector of ast actions and the last
 * node in the plan tree
 */
std::tuple<cocg::CoCGState, std::vector<cocg_ast::Action::SharedPtr>,
           cocg::ContPlanNode::SharedPtr>
traverse_contingent_planning_tree(const cocg::CoCGState& init_state,
                                  cocg::ContPlanNode::SharedPtr root,
                                  cocg::DomainExpert& domain_expert);

/**
 * @brief output the tree infl
 * @param root root node of the plan tree
 */
void print_cont_plan_tree(ContPlanNode::SharedPtr root);
}  // namespace cocg

#endif  // COCG_CORE_COCG_UTILS_H_
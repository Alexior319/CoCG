#ifndef COCG_CORE_COCG_UTILS_H_
#define COCG_CORE_COCG_UTILS_H_

#include <memory>
#include <tuple>

#include "cocg_core/cocg_problem_expert.h"
#include "cocg_core/cocg_problem_expert_utils.h"
#include "cocg_core/cont_plan_node.h"

namespace cocg {

/**
 * @brief apply the effects of an actuation action to a state
 * @param init_state the initial state, stored in a proble expert
 * @param action the actuation action
 * @param new_state whether to create a new state or modify the input state
 * @return pointer to the goal state, stored in a proble expert
 */
std::shared_ptr<cocg::ProblemExpert> apply_actuation_action(
    std::shared_ptr<cocg::ProblemExpert> init_state,
    const cocg_ast::Action& action, bool new_state = false);

/**
 * @brief apply the effects of a sensing action to a state
 * @param init_state the initial state
 * @param action the sensing action
 * @param sensing_result the result of the sensing action, true or false
 * @param new_state whether to create a new state or modify the input state
 * @return the goal state
 */
std::shared_ptr<cocg::ProblemExpert> apply_sensing_action(
    std::shared_ptr<cocg::ProblemExpert> init_state,
    const cocg_ast::Action& action, bool sensing_result,
    bool new_state = false);

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
std::tuple<std::shared_ptr<cocg::ProblemExpert>,
           std::vector<cocg_ast::Action::SharedPtr>,
           cocg::ContPlanNode::SharedPtr>
traverse_contingent_planning_tree(
    const std::shared_ptr<cocg::ProblemExpert> init_state,
    cocg::ContPlanNode::SharedPtr root, cocg::DomainExpert& domain_expert);

/**
 * @brief output the tree infl
 * @param root root node of the plan tree
 */
void print_cont_plan_tree(ContPlanNode::SharedPtr root);
}  // namespace cocg

#endif  // COCG_CORE_COCG_UTILS_H_
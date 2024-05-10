#ifndef COCG_CORE_COCG_UTILS_H_
#define COCG_CORE_COCG_UTILS_H_

#include <tuple>

#include "cocg_core/cocg_problem_expert.h"
#include "cocg_core/cont_plan_node.h"

namespace cocg {
/**
 * @brief traverse a contingent planning tree until the next sensing node or the
 * nullptr
 * @param init_state the initial state
 * @param root the root of the contingent planning tree
 * @return a tuple with the goal state, the vector of nodes and the last node
 */
std::tuple<cocg_ast::Tree, std::vector<cocg::ContPlanNode::SharedPtr>,
           cocg::ContPlanNode::SharedPtr>
traverse_contingent_planning_tree(cocg_ast::Tree init_state,
                                  cocg::ContPlanNode::SharedPtr root) {
  cocg_ast::Tree goal_state = init_state;
  std::vector<cocg::ContPlanNode::SharedPtr> nodes;
  cocg::ContPlanNode::SharedPtr node = root;
  while (node != nullptr) {
    if (node->true_node != node->false_node) break;
    // TODO
    nodes.push_back(node);
  }
  return {goal_state, nodes, node};
}
}  // namespace cocg

#endif  // COCG_CORE_COCG_UTILS_H_
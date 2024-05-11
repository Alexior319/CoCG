#ifndef COCG_CORE_SUBGRAPH_H_
#define COCG_CORE_SUBGRAPH_H_

#include <map>
#include <memory>
#include <vector>

#include "cocg_ast/action.h"
#include "cocg_core/cocg_problem_expert.h"
#include "cocg_core/cocg_utils.h"

namespace cocg {
struct SubGraphNode {
  float t0_;
  int layers_cnt_;
  std::map<float, std::vector<cocg_ast::Action>> actions_layers_;
  std::shared_ptr<SubGraphNode> next_true_;
  std::shared_ptr<SubGraphNode> next_false_;
};

/**
 * @brief build the cocg graph recursively
 * @param init_state the initial state
 * @param mid_goal_state the mid goal state
 * @param actions the actions sets between the initial state and the mid goal
 * state
 * @param node the current sensing node or null node after the goal state
 * @param t0 the starting time of the current subgraph
 * @param domain_expert the domain expert
 * @return the root subgraph
 */
std::shared_ptr<SubGraphNode> build_cocg_subgraph(
    const CoCGState& init_state, const CoCGState& mid_goal_state,
    std::vector<cocg_ast::Action::SharedPtr> actions,
    cocg::ContPlanNode::SharedPtr node, float t0,
    cocg::DomainExpert& domain_expert);

/**
 * @brief compute the planning graph using GraphPlan algorithm or else
 * @param init_state the initial state
 * @param goal_state the goal state
 * @param actions the grounded actions sets between the initial state and the
 * goal state
 * @param domain_expert the domain expert
 */
std::vector<std::vector<cocg_ast::Action>> compute_planning_graph(
    const CoCGState& init_state, const CoCGState& goal_state,
    std::vector<cocg_ast::Action::SharedPtr> actions,
    cocg::DomainExpert& domain_expert);

}  // namespace cocg

#endif  // COCG_CORE_SUBGRAPH_H_

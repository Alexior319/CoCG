#ifndef COCG_CORE_SUBGRAPH_H_
#define COCG_CORE_SUBGRAPH_H_

#include <map>
#include <memory>
#include <tuple>
#include <vector>

#include "cocg_ast/action.h"
#include "cocg_core/cocg_graphplan.h"
#include "cocg_core/cocg_problem_expert.h"
#include "cocg_core/cocg_utils.h"

namespace cocg {
struct SubGraphNode {
  float t0_;
  int layers_cnt_;
  std::map<uint32_t, std::vector<cocg_ast::Action>> actions_layers_;
  std::shared_ptr<SubGraphNode> next_true_;
  std::shared_ptr<SubGraphNode> next_false_;
};

/**
 * @brief traverse a contingent planning tree until the next sensing node or the
 * nullptr
 * @param init_state the initial state
 * @param root the root of the contingent planning tree
 * @return a tuple with the goal state, the vector of ast actions and the last
 * node in the plan tree
 */
std::tuple<std::shared_ptr<cocg::ProblemExpert>, std::vector<cocg_ast::Action>,
           cocg::ContPlanNode::SharedPtr>
traverse_contingent_planning_tree(
    const std::shared_ptr<cocg::ProblemExpert> init_state,
    cocg::ContPlanNode::SharedPtr root, cocg::DomainExpert& domain_expert);

/**
 * @brief build the cocg graph recursively
 * @param init_state the problem expert that contains the initial state
 * @param goal_state the problem expert that contains the goal state
 * @param actions the actions sets between the initial state and the mid goal
 * state
 * @param node the current sensing node or null node after the goal state
 * @param t0 the starting time of the current subgraph
 * @param domain_expert the domain expert
 * @return the root subgraph
 */
std::shared_ptr<SubGraphNode> build_cocg_subgraph(
    std::shared_ptr<cocg::ProblemExpert> init_state,
    std::shared_ptr<cocg::ProblemExpert> goal_state,
    std::vector<cocg_ast::Action> actions,
    cocg::ContPlanNode::SharedPtr node, float t0,
    cocg::DomainExpert& domain_expert);

/**
 * @brief compute the planning graph using GraphPlan algorithm or else
 * @param init_state the problem expert that contains the initial state
 * @param goal_state the problem expert that contains the goal state
 * @param actions the grounded actions sets between the initial state and the
 * @param domain_expert the domain expert
 * @return action layers
 */
std::vector<std::vector<cocg_ast::Action>> compute_planning_graph(
    std::shared_ptr<cocg::ProblemExpert> init_state,
    std::shared_ptr<cocg::ProblemExpert> goal_state,
    const std::vector<cocg_ast::Action>& actions,
    const cocg::DomainExpert& domain_expert);
}  // namespace cocg

#endif  // COCG_CORE_SUBGRAPH_H_

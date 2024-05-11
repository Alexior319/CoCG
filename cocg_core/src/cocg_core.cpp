#include "cocg_core/cocg_core.h"

namespace cocg {
std::shared_ptr<SubGraphNode> build_cocg_subgraph(
    const CoCGState& init_state, const CoCGState& mid_goal_state,
    std::vector<cocg_ast::Action::SharedPtr> actions,
    cocg::ContPlanNode::SharedPtr node, float t0,
    cocg::DomainExpert& domain_expert) {
  std::shared_ptr<SubGraphNode> ret = std::make_shared<SubGraphNode>();
  ret->t0_ = t0;

  std::vector<std::vector<cocg_ast::Action>> action_layers =
      compute_planning_graph(init_state, mid_goal_state, actions,
                             domain_expert);

  int i = 1, k = action_layers.size();
  for (; i <= k; i++) {
    ret->actions_layers_[i] = action_layers[i];
  }

  ret->layers_cnt_ = k + 1;
  if (node != nullptr) {
    cocg_ast::Action::SharedPtr action =
        convert_plan_node_to_ast(node, domain_expert);
    ret->actions_layers_[k + 1] = {*action};

    // traverse the true part
    CoCGState next_true_init_state =
        apply_sensing_action(mid_goal_state, *action, true);
    std::tuple<cocg::CoCGState, std::vector<cocg_ast::Action::SharedPtr>,
               cocg::ContPlanNode::SharedPtr>
        next_true_tuple = traverse_contingent_planning_tree(
            next_true_init_state, node->true_node, domain_expert);
    ret->next_true_ = build_cocg_subgraph(
        next_true_init_state, std::get<0>(next_true_tuple),
        std::get<1>(next_true_tuple), std::get<2>(next_true_tuple),
        t0 + ret->layers_cnt_, domain_expert);

    // traverse the false part
    CoCGState next_false_init_state =
        apply_sensing_action(mid_goal_state, *action, false);
    std::tuple<cocg::CoCGState, std::vector<cocg_ast::Action::SharedPtr>,
               cocg::ContPlanNode::SharedPtr>
        next_false_tuple = traverse_contingent_planning_tree(
            next_false_init_state, node->false_node, domain_expert);
    ret->next_true_ = build_cocg_subgraph(
        next_false_init_state, std::get<0>(next_true_tuple),
        std::get<1>(next_true_tuple), std::get<2>(next_true_tuple),
        t0 + ret->layers_cnt_, domain_expert);
  } else {
    ret->actions_layers_[k + 1] = {};
  }
  return ret;
}

std::vector<std::vector<cocg_ast::Action>> compute_planning_graph(
    const CoCGState& init_state, const CoCGState& goal_state,
    std::vector<cocg_ast::Action::SharedPtr> actions,
    cocg::DomainExpert& domain_expert) {
  // TODO: compute parallelized planning graph
}
}  // namespace cocg

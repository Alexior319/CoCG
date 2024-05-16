#include "cocg_core/cocg_core.h"

namespace cocg {

std::tuple<std::shared_ptr<cocg::ProblemExpert>, std::vector<cocg_ast::Action>,
           cocg::ContPlanNode::SharedPtr>
traverse_contingent_planning_tree(
    const std::shared_ptr<cocg::ProblemExpert> init_state,
    cocg::ContPlanNode::SharedPtr root, cocg::DomainExpert& domain_expert) {
  std::shared_ptr<cocg::ProblemExpert> goal_state =
      std::make_shared<cocg::ProblemExpert>(init_state);

  std::vector<cocg_ast::Action> mid_actions;
  cocg::ContPlanNode::SharedPtr last_node = root;
  while (last_node != nullptr) {
    // sensing action or null action node
    if (last_node->true_node != last_node->false_node) break;

    // action ast
    cocg_ast::Action::SharedPtr action_ast =
        convert_plan_node_to_ast(last_node, domain_expert);
    mid_actions.push_back(*action_ast);

    goal_state = apply_actuation_action(goal_state, *action_ast);

    last_node = last_node->true_node;
  }
  return {goal_state, mid_actions, last_node};
}

std::shared_ptr<SubGraphNode> build_cocg_subgraph(
    std::shared_ptr<cocg::ProblemExpert> init_state,
    std::shared_ptr<cocg::ProblemExpert> goal_state,
    std::vector<cocg_ast::Action> actions, cocg::ContPlanNode::SharedPtr node,
    float t0, cocg::DomainExpert& domain_expert) {
  std::shared_ptr<SubGraphNode> ret = std::make_shared<SubGraphNode>();
  ret->t0_ = t0;

  std::vector<std::vector<cocg_ast::Action>> action_layers =
      compute_planning_graph(init_state, goal_state, actions, domain_expert);

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
    std::shared_ptr<cocg::ProblemExpert> next_true_mid_expert =
        apply_sensing_action(init_state, *action, true, true);
    std::tuple<std::shared_ptr<cocg::ProblemExpert>,
               std::vector<cocg_ast::Action>, cocg::ContPlanNode::SharedPtr>
        next_true_tuple = traverse_contingent_planning_tree(
            next_true_mid_expert, node->true_node, domain_expert);
    ret->next_true_ = build_cocg_subgraph(
        next_true_mid_expert, std::get<0>(next_true_tuple),
        std::get<1>(next_true_tuple), std::get<2>(next_true_tuple),
        t0 + ret->layers_cnt_, domain_expert);

    // traverse the false part
    std::shared_ptr<cocg::ProblemExpert> next_false_mid_expert =
        apply_sensing_action(init_state, *action, false, true);
    std::tuple<std::shared_ptr<cocg::ProblemExpert>,
               std::vector<cocg_ast::Action>, cocg::ContPlanNode::SharedPtr>
        next_false_tuple = traverse_contingent_planning_tree(
            next_false_mid_expert, node->false_node, domain_expert);
    ret->next_true_ = build_cocg_subgraph(
        next_false_mid_expert, std::get<0>(next_false_tuple),
        std::get<1>(next_false_tuple), std::get<2>(next_false_tuple),
        t0 + ret->layers_cnt_, domain_expert);
  } else {
    ret->actions_layers_[k + 1] = {};
  }
  return ret;
}

std::vector<std::vector<cocg_ast::Action>> compute_planning_graph(
    std::shared_ptr<cocg::ProblemExpert> init_state,
    std::shared_ptr<cocg::ProblemExpert> goal_state,
    const std::vector<cocg_ast::Action> actions,
    const cocg::DomainExpert& domain_expert) {
  std::vector<std::vector<cocg_ast::Action>> ret_action_layers;

  // step 1: create graph

  // step 2: 
  return ret_action_layers;
}
}  // namespace cocg

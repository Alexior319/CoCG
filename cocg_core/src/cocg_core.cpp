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
    const std::vector<cocg_ast::Action>& actions,
    const cocg::DomainExpert& domain_expert) {
  // Return value
  std::vector<std::vector<cocg_ast::Action>> ret_action_layers;
  // A new propositon-action graph
  PAGraph pa_graph;
  bool solved = false;

  // Step 1: build the first(No.0) state and action layer
  StateLayerMap init_state_layer;
  for (const auto& pred : init_state->getPredicates()) {
    std::shared_ptr<cocg_ast::Tree> fact =
        std::make_shared<cocg_ast::Tree>(pred);
    std::shared_ptr<PGStateNode> state_node =
        std::make_shared<PGStateNode>(*fact);
    init_state_layer[get_fact_string(*fact)] = state_node;
  }
  pa_graph.state_layers.push_back(init_state_layer);
  pa_graph.action_layers.push_back({});
  pa_graph.state_mutex_layers.push_back({});
  pa_graph.action_mutex_layers.push_back({});
  pa_graph.layers++;
  // get the goal state in string format
  std::vector<std::string> goals;
  for (const auto& pred : goal_state->getPredicates()) {
    goals.push_back(parser::pddl::toString(pred));
  }

#ifdef OUTPUT_DEBUG_INFO
  std::cout << "[PAGraph] Initial state layer: ";
  print_state_layer(pa_graph.state_layers[0]);
#endif

  // Step 2: expand the graph until the goal facts all appear in the last state,
  // without state mutex
  create_init_graph(goals, pa_graph, actions);

  // Step 3: Extract the graph to get a solution without any mutex
  while (true) {
    std::tuple<bool, std::vector<ActionLayerMap>> ret =
        extract_solution(goals, pa_graph);
    solved = std::get<0>(ret);
    if (solved) {
      auto action_node_layers = std::get<1>(ret);
      for (int i = 0; i < action_node_layers.size(); i++) {
        for (const auto& it : action_node_layers[i]) {
          ret_action_layers[i].push_back(it.second->action_);
        }
      }
      break;
    } else {
      create_graph_layer(pa_graph, actions);
    }
  }

  return ret_action_layers;
}
}  // namespace cocg

#include "cocg_core/cocg_core.h"

namespace cocg {

std::tuple<std::shared_ptr<cocg::ProblemExpert>, std::vector<cocg_ast::Action>,
           cocg::ContPlanNode::SharedPtr>
traverse_contingent_planning_tree(
    const std::shared_ptr<cocg::ProblemExpert> init_state,
    cocg::ContPlanNode::SharedPtr root,
    const cocg::DomainExpert& domain_expert) {
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
    float t0, const cocg::DomainExpert& domain_expert) {
  std::shared_ptr<SubGraphNode> ret = std::make_shared<SubGraphNode>();
  ret->t0_ = t0;

  std::vector<std::vector<cocg_ast::Action>> action_layers =
      compute_planning_graph(init_state, goal_state, actions, domain_expert);

#ifdef OUTPUT_DEBUG_INFO
  std::cout << "------------------------------------\n";
  std::cout << "[SubGraphNode] Built subgraph, t0: " << t0 << "\n";
  print_action_layers(action_layers);
  std::cout << "------------------------------------\n";
#endif

  int k = action_layers.size();
  action_layers.swap(ret->actions_layers_);

  // plus the null layer or the sensing node layer
  ret->layers_cnt_ = k + 1;
  ret->actions_layers_.push_back({});
  if (node != nullptr) {
    cocg_ast::Action::SharedPtr action =
        convert_plan_node_to_ast(node, domain_expert);
    ret->actions_layers_[k] = {*action};

    // traverse the true part
    std::shared_ptr<cocg::ProblemExpert> next_true_mid_expert =
        apply_sensing_action(goal_state, *action, true, true);
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
        apply_sensing_action(goal_state, *action, false, true);
    std::tuple<std::shared_ptr<cocg::ProblemExpert>,
               std::vector<cocg_ast::Action>, cocg::ContPlanNode::SharedPtr>
        next_false_tuple = traverse_contingent_planning_tree(
            next_false_mid_expert, node->false_node, domain_expert);
    ret->next_false_ = build_cocg_subgraph(
        next_false_mid_expert, std::get<0>(next_false_tuple),
        std::get<1>(next_false_tuple), std::get<2>(next_false_tuple),
        t0 + ret->layers_cnt_, domain_expert);
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

#ifdef OUTPUT_DEBUG_INFO
  std::cout << "---------------------------------------------" << std::endl;
  std::cout << "[PAGraph] applicable actions in this graph: ";
  for (int j = 0; j < actions.size(); j++) {
    std::cout << get_grounded_action_string(actions[j]) << " ";
  }
  std::cout << std::endl;
#endif

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
  std::cout << "-----------------------------\n";
  std::cout << "[PAGraph] Initializing layer, No.0\n";
  std::cout << "[PAGraph] State Layer: ";
  print_state_layer(pa_graph.state_layers[0]);
  std::cout << "[PAGraph] Action Layer: ";
  print_action_layer(pa_graph.action_layers[0]);
#endif

  // Step 2: expand the graph until the goal facts all appear in the last state,
  // without state mutex
  create_init_graph(goals, pa_graph, actions);

  // Step 3: Extract the graph to get a solution without any mutex
  // graph layers leq actions size (worst: sequential execution)
  while (true) {
    std::tuple<bool, std::vector<ActionLayerMap>> ret =
        extract_solution(goals, pa_graph);
    solved = std::get<0>(ret);
    if (solved) {
      auto action_node_layers = std::get<1>(ret);
      ret_action_layers.resize(action_node_layers.size());
      for (int i = 0; i < action_node_layers.size(); i++) {
        for (const auto& it : action_node_layers[i]) {
          // ignore the noop actions
          if (it.second->is_noop_) continue;
          ret_action_layers[i].push_back(it.second->action_);
        }
      }
#ifdef OUTPUT_DEBUG_INFO
      for (int i = 0; i < ret_action_layers.size(); i++) {
        std::cout << "[PAGraph] Actions in layer No." << i << ": ";
        for (int j = 0; j < ret_action_layers[i].size(); j++) {
          std::cout << cocg::get_grounded_action_string(ret_action_layers[i][j])
                    << " ";
        }
        std::cout << std::endl;
      }
      std::cout << "-----------------------------------------" << std::endl;
#endif
      break;
    } else {
      std::cout << "[PAGraph] Extracting failed, expand the graph..."
                << std::endl;
      if (pa_graph.layers > actions.size()) {
        std::cout << "--> [PAGraph] The layers of the graph has exceeded the "
                     "actions num when extracting. <=================="
                  << std::endl;
        break;
      }
      create_graph_layer(pa_graph, actions);
    }
  }
  // the first layer is empty, remove it
  if (ret_action_layers.size() > 0)
    ret_action_layers.erase(ret_action_layers.begin());

  return ret_action_layers;
}

void print_cocg_tree(const std::string& prefix,
                     const std::shared_ptr<SubGraphNode> node, bool isLeft,
                     int depth = 0) {
  if (node != nullptr) {
    std::cout << prefix;

    std::cout << (isLeft ? "├─t " : "└─f ");
    std::cout << " depth: " << depth << ", layers: " << node->layers_cnt_
              << std::endl;

    // print the value(layered actions in the graph) of the node
    print_action_layers(node->actions_layers_, depth, "|   ");

    // enter the next tree level - left and right branch
    print_cocg_tree(prefix + (isLeft ? "│   " : "    "), node->next_true_, true,
                    depth + 1);
    print_cocg_tree(prefix + (isLeft ? "│   " : "    "), node->next_false_,
                    false, depth + 1);
  }
}

void print_cocg_graph(std::shared_ptr<SubGraphNode> graph_root_node) {
  std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n";
  print_cocg_tree("", graph_root_node, true, 1);
  std::cout << "**************************************\n";
}

}  // namespace cocg

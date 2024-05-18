
#include "cocg_core/cocg_graphplan.h"

namespace cocg {

void create_init_graph(const std::vector<std::string>& goals, PAGraph& pa_graph,
                       const std::vector<cocg_ast::Action>& actions) {
  // expand the graph until all goals are contained in the last layer
  while (!goal_contained_in_state_layer(
      goals, pa_graph.state_layers[pa_graph.layers])) {
    create_graph_layer(pa_graph, actions);
  }

  // if the goals in the last state layer are mutex, continue expanding
  while (exist_mutex_in_goal_layer(goals, pa_graph)) {
    create_graph_layer(pa_graph, actions);
  }
}

void create_graph_layer(PAGraph& pa_graph,
                        const std::vector<cocg_ast::Action>& actions) {
  // TODO
  uint32_t cur_layer = pa_graph.layers, next_layer = cur_layer + 1;
  StateLayerMap next_state_layer;
  ActionLayerMap next_action_layer;
  StateMutexMap next_state_mutex_map;
  ActionMutexMap next_action_mutex_map;

  // copy facts over to next state layer
  for (const auto& it : pa_graph.state_layers[cur_layer]) {
    // just copy fact, ignore the connections
    std::shared_ptr<PGStateNode> new_node =
        std::make_shared<PGStateNode>(it.second->fact_);
    next_state_layer[it.first] = new_node;
  }

  // make the noop layer
  for (auto& it : pa_graph.state_layers[cur_layer]) {
    std::shared_ptr<PGActionNode> noop_node = std::make_shared<PGActionNode>();
    noop_node->action_.preconditions = it.second->fact_;
    noop_node->action_.effects = it.second->fact_;

    // noop node name: "_noop" + fact
    std::string noop_node_name = "_noop" + it.first;
    noop_node->action_.name = noop_node_name;
    next_action_layer[noop_node_name] = noop_node;

    // make connections, seemed not necessary since string+map is ok
    it.second->after_action_nodes_.insert(noop_node);
    noop_node->before_state_nodes_.insert(it.second);
    pa_graph.state_layers[next_layer][it.first]->before_action_nodes_.insert(
        noop_node);
  }

  // insert the applicable actions into next layer
  for (const auto& action : actions) {
    // split the preconditions into a vector of facts
    std::vector<std::string> precond_facts, effect_facts;
    parser::pddl::get_facts_string(action.preconditions, precond_facts);

    bool is_applicable = true;
    for (const auto& precond : precond_facts) {
      if (pa_graph.state_layers[cur_layer].find(precond) == pa_graph.state_layers[cur_layer].end()) {
        is_applicable = false;
        break;
      }
    }

    // if applicable, insert the action node and their effects to next layer
    if (is_applicable) {
      std::shared_ptr<PGActionNode> a_node = std::make_shared<PGActionNode>(action);
      // make precondition connections
      for (const auto& precond : a_node->precond_facts_) {
        a_node->before_state_nodes_.insert(pa_graph.state_layers[cur_layer][precond]);
      }

      // apply effects
      for (const auto& effect : a_node->effect_facts_) {
        // TODO
      }
    }
  }

  // find mutex actions

  // find mutex facts
}

std::tuple<bool, std::vector<ActionLayerMap>> extract_solution(
    const std::vector<std::string>& goals, const PAGraph& pa_graph,
    const std::vector<cocg_ast::Action>& actions) {
  std::tuple<bool, std::vector<ActionLayerMap>> ret;
  // TODO

  return ret;
}

bool goal_contained_in_state_layer(const std::vector<std::string>& goals,
                                   const StateLayerMap& state_layer) {
  bool goal_contained = false;
  // TODO

  return goal_contained;
}

bool exist_mutex_in_goal_layer(const std::vector<std::string>& goals,
                               const PAGraph& pa_graph) {
  bool exist_mutex = false;
  // TODO

  return exist_mutex;
}
}  // namespace cocg
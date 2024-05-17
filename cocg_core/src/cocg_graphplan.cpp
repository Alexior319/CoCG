
#include "cocg_core/cocg_graphplan.h"

namespace cocg {

void create_init_graph(const std::vector<std::string>& goals, PAGraph& pa_graph,
                       const std::vector<cocg_ast::Action>& actions) {
  // expand the graph until all goals are contained in the last layer
  while (!goal_contained_in_state_layer(
      goals, pa_graph.state_layers[pa_graph.layers])) {
    create_graph_layer(goals, pa_graph, actions);
  }

  // if the goals in the last state layer are mutex, continue expanding
  while (exist_mutex_in_goal_layer(goals, pa_graph)) {
    create_graph_layer(goals, pa_graph, actions);
  }
}

void create_graph_layer(const std::vector<std::string>& goals,
                        PAGraph& pa_graph,
                        const std::vector<cocg_ast::Action>& actions) {
  // TODO
}

std::tuple<bool, std::vector<ActionLayerMap>> extract_solution(
    const std::vector<std::string>& goals, PAGraph& pa_graph,
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
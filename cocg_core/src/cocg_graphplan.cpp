
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
      if (pa_graph.state_layers[cur_layer].find(precond) ==
          pa_graph.state_layers[cur_layer].end()) {
        is_applicable = false;
        break;
      }
    }

    // if applicable, insert the action node and their effects to next layer
    if (is_applicable) {
      std::shared_ptr<PGActionNode> a_node =
          std::make_shared<PGActionNode>(action);
      // make precondition connections
      for (const auto& precond : a_node->precond_facts_) {
        a_node->before_state_nodes_.insert(
            pa_graph.state_layers[cur_layer][precond]);
      }

      // apply effects and make connections
      for (const auto& effect : a_node->effect_facts_) {
        auto it = pa_graph.state_layers[next_layer].find(effect);
        if (it == pa_graph.state_layers[next_layer].end()) {
          std::shared_ptr<PGStateNode> new_node =
              std::make_shared<PGStateNode>(effect);
          next_state_layer[effect] = new_node;
          it = next_state_layer.find(effect);
        }
        it->second->before_action_nodes_.insert(a_node);
        a_node->after_state_nodes_.insert(it->second);
      }
    }
  }

  // find mutex actions
  for (auto it = next_action_layer.begin(); it != next_action_layer.end();
       ++it) {
    for (auto it2 = std::next(it); it2 != next_action_layer.end(); ++it2) {
      // if mutex, insert into mutex map
      // 1. find competing needs
      for (auto a1 : it->second->before_state_nodes_) {
        for (auto a2 : it2->second->before_state_nodes_) {
          if (negated_facts(a1->get_fact(), a2->get_fact())) {
            next_action_mutex_map[it->first].insert(it2->first);
            next_action_mutex_map[it2->first].insert(it->first);
          }
        }
      }

      // 2. find inconsistent effects
      for (auto a1 : it->second->after_state_nodes_) {
        for (auto a2 : it2->second->after_state_nodes_) {
          if (negated_facts(a1->get_fact(), a2->get_fact())) {
            next_action_mutex_map[it->first].insert(it2->first);
            next_action_mutex_map[it2->first].insert(it->first);
          }
        }
      }

      // 3. find interference
      for (auto a1 : it->second->before_state_nodes_) {
        for (auto a2 : it2->second->after_state_nodes_) {
          if (negated_facts(a1->get_fact(), a2->get_fact())) {
            next_action_mutex_map[it->first].insert(it2->first);
            next_action_mutex_map[it2->first].insert(it->first);
          }
        }
      }
    }
  }

  // find mutex facts
  for (auto it = next_state_layer.begin(); it != next_state_layer.end(); ++it) {
    for (auto it2 = std::next(it); it2 != next_state_layer.end(); ++it2) {
      // if mutex, insert into mutex map
      // 1. get negated literals
      if (negated_facts(it->first, it2->first)) {
        next_state_mutex_map[it->first].insert(it2->first);
        next_state_mutex_map[it2->first].insert(it->first);
      } else {
        // 2. inconsistent support: check if the actions that lead to the facts
        // are mutex
        for (auto a1 : it->second->before_action_nodes_) {
          for (auto a2 : it2->second->before_action_nodes_) {
            if (next_action_mutex_map[a1->action_.name].find(
                    a2->action_.name) !=
                next_action_mutex_map[a1->action_.name].end()) {
              next_state_mutex_map[it->first].insert(it2->first);
              next_state_mutex_map[it2->first].insert(it->first);
            }
          }
        }
      }
    }
  }
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
  for (const auto& goal : goals) {
    if (state_layer.find(goal) == state_layer.end()) {
      goal_contained = false;
      break;
    }
    goal_contained = true;
  }
  return goal_contained;
}

bool exist_mutex_in_goal_layer(const std::vector<std::string>& goals,
                               const PAGraph& pa_graph) {
  bool exist_mutex = false;
  for (auto goal1 = goals.begin(); goal1 != goals.end(); ++goal1) {
    for (auto goal2 = std::next(goal1); goal2 != goals.end(); ++goal2) {
      if (two_facts_mutex_in_layer(*goal1, *goal2,
                                   pa_graph.state_mutex_layers.back())) {
        exist_mutex = true;
        break;
      }
    }
    if (exist_mutex) {
      break;
    }
  }
  return exist_mutex;
}

bool two_facts_mutex_in_layer(const std::string& fact1,
                              const std::string& fact2,
                              const StateMutexMap& state_mutex_map) {
  bool mutex = false;
  if (state_mutex_map.find(fact1) != state_mutex_map.end() &&
      state_mutex_map.find(fact2) != state_mutex_map.end()) {
    if (state_mutex_map.at(fact1).find(fact2) !=
        state_mutex_map.at(fact1).end()) {
      mutex = true;
    }
  }
  return mutex;
}
}  // namespace cocg

#include "cocg_core/cocg_graphplan.h"

namespace cocg {

void create_init_graph(const std::vector<std::string>& goals, PAGraph& pa_graph,
                       const std::vector<cocg_ast::Action>& actions) {
  // expand the graph until all goals are contained in the last layer
  while (!goal_contained_in_state_layer(
      goals, pa_graph.state_layers[pa_graph.layers - 1])) {
    if (pa_graph.layers > actions.size()) {
      std::cout << "--> [PAGraph] The layers of the graph has exceeded the "
                   "actions num. <=================="
                << std::endl;
      break;
    }
    create_graph_layer(pa_graph, actions);
  }

#ifdef OUTPUT_DEBUG_INFO
  std::cout
      << "[PAGraph] Goals all found in the last state layer, total layers: "
      << pa_graph.layers << std::endl;
#endif

  // if the goals in the last state layer are mutex, continue expanding
  while (exist_mutex_in_goal_layer(goals, pa_graph)) {
    if (pa_graph.layers > actions.size()) {
      std::cout << "--> [PAGraph] The layers of the graph has exceeded the "
                   "actions num. <=================="
                << std::endl;
      break;
    }
    create_graph_layer(pa_graph, actions);
  }

#ifdef OUTPUT_DEBUG_INFO
  std::cout << "[PAGraph] Init graph built, total layers: " << pa_graph.layers
            << std::endl;
#endif
}

void create_graph_layer(PAGraph& pa_graph,
                        const std::vector<cocg_ast::Action>& actions) {
  uint32_t cur_layer = pa_graph.layers - 1, next_layer = cur_layer + 1;
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

    // noop node name: "(_noop " + fact + ")"
    std::string noop_node_name = "(_noop " + it.first + ")";
    noop_node->action_.name = noop_node_name;
    noop_node->is_noop_ = true;
    next_action_layer[noop_node_name] = noop_node;

    // make connections, seemed not necessary since string+map is ok
    it.second->after_action_nodes_.insert(noop_node);
    noop_node->before_state_nodes_.insert(it.second);
    next_state_layer[it.first]->before_action_nodes_.insert(noop_node);
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
      next_action_layer[a_node->get_action()] = a_node;
      // make precondition connections
      for (const auto& precond : a_node->precond_facts_) {
        a_node->before_state_nodes_.insert(
            pa_graph.state_layers[cur_layer][precond]);
      }

      // apply effects and make connections
      for (const auto& effect : a_node->effect_facts_) {
        auto s_node_pair = next_state_layer.find(effect);
        // s_node not found, create a new one
        if (s_node_pair == next_state_layer.end()) {
          std::shared_ptr<PGStateNode> new_node =
              std::make_shared<PGStateNode>(effect);
          next_state_layer[effect] = new_node;
          // make connections
          a_node->after_state_nodes_.insert(new_node);
          new_node->before_action_nodes_.insert(a_node);
        } else {
          // s_node found, make connections
          a_node->after_state_nodes_.insert(s_node_pair->second);
          s_node_pair->second->before_action_nodes_.insert(a_node);
        }
      }
    }
  }

#ifdef OUTPUT_DEBUG_INFO
  std::cout << "-----------------------------------" << std::endl;
  std::cout << "[PAGraph] Building next layer, No." << next_layer << std::endl;
  std::cout << "[PAGraph] Next action layer: ";
  print_action_layer(next_action_layer);
  std::cout << "[PAGraph] Next state layer: ";
  print_state_layer(next_state_layer);
#endif

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
      for (auto a1 : it->second->after_state_nodes_) {
        for (auto a2 : it2->second->before_state_nodes_) {
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
        // 2. inconsistent support: check if all the actions that lead to the
        // facts are mutex, if one pair of actions are not mutex, then the facts
        // are not
        bool exist_pair_not_mutex = false;
        bool exist_mutex_pairs = false;
        for (const auto& a1 : it->second->before_action_nodes_) {
          for (const auto& a2 : it2->second->before_action_nodes_) {
            if (next_action_mutex_map[a1->get_action()].find(
                    a2->get_action()) ==
                next_action_mutex_map[a1->get_action()].end()) {
              // at least one pair of actions are not mutex
              exist_pair_not_mutex = true;
              break;
            } else {
              exist_mutex_pairs = true;
            }
          }
          if (exist_pair_not_mutex) {
            break;
          }
        }
        // all the action pairs are mutex, then the facts are mutex
        if (!exist_pair_not_mutex && exist_mutex_pairs) {
          next_state_mutex_map[it->first].insert(it2->first);
          next_state_mutex_map[it2->first].insert(it->first);
        }
      }
    }
  }

  // expand the graph
  pa_graph.state_layers.push_back(next_state_layer);
  pa_graph.action_layers.push_back(next_action_layer);
  pa_graph.state_mutex_layers.push_back(next_state_mutex_map);
  pa_graph.action_mutex_layers.push_back(next_action_mutex_map);
  pa_graph.layers++;
}

// TODO: BUG HERE
std::tuple<bool, bool> extract_backward_from_layer(
    const std::unordered_set<std::string>& cur_goals, const PAGraph& pa_graph,
    uint32_t cur_layer, std::vector<ActionLayerMap>& extraction_layers,
    std::unordered_set<std::string> prev_goals) {
  // backtracking algo: traverse layers until the first layer
  // If we are can go to initial state layer, we found the solution
  if (cur_layer == 0) {
#ifdef OUTPUT_DEBUG_INFO
    std::cout << "[PAGraph] Extracting to the initial layer, solved!"
              << std::endl;
#endif
    return std::make_tuple<bool, bool>(true, true);
  }

  bool found_extraction = false;
  bool goals_extracted_in_cur_layer = false;
  std::unordered_set<std::string> remained_goals = cur_goals;

  // goals satisfied in current layer, extract the previous layer
  if (cur_goals.size() == 0) {
    goals_extracted_in_cur_layer = true;
#ifdef OUTPUT_DEBUG_INFO
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "[PAGraph] Goals satisfied in layer No." << cur_layer
              << ", extracting the previous layer" << std::endl;
#endif
    auto prev_ret = extract_backward_from_layer(
        prev_goals, pa_graph, cur_layer - 1, extraction_layers);
    found_extraction = std::get<0>(prev_ret);
  } else {
    // find the actions that can achieve the goals
    for (const auto& goal : cur_goals) {
      auto state_node_pair = pa_graph.state_layers[cur_layer].find(goal);
      if (state_node_pair != pa_graph.state_layers[cur_layer].end()) {
        for (const auto& action_node :
             state_node_pair->second->before_action_nodes_) {
          bool mutex = false;
          // the chosen action is mutex with extracted actions
          for (const auto& action_node2 : extraction_layers[cur_layer]) {
            if (two_actions_mutex_in_layer(
                    action_node->get_action(), action_node2.first,
                    pa_graph.action_mutex_layers[cur_layer])) {
#ifdef OUTPUT_DEBUG_INFO
              std::cout << "[PAGraph] Action: " << action_node->get_action()
                        << " mutex with extracted actions, choosing another...";
              std::cout << std::endl;
#endif
              mutex = true;
              break;
            }
          }
          if (!mutex) {
            // erase the goal in this layer
            remained_goals.erase(goal);
            // add the precondition facts to the prev_goals
            for (const auto& precond : action_node->precond_facts_) {
              prev_goals.insert(precond);
            }

            // add the action to the extraction map
            extraction_layers[cur_layer][action_node->get_action()] =
                action_node;

#ifdef OUTPUT_DEBUG_INFO
            std::cout << ">>> [PAGraph] Action found: "
                      << action_node->get_action() << ", for goal: " << goal
                      << ", extracting remained goals: ";
            for (const auto& g : remained_goals) {
              std::cout << g << " ";
            }
            std::cout << std::endl;
#endif

            // extract the current layer with the remained goals
            auto cur_remained_ret =
                extract_backward_from_layer(remained_goals, pa_graph, cur_layer,
                                            extraction_layers, prev_goals);
            found_extraction = std::get<0>(cur_remained_ret);
            goals_extracted_in_cur_layer = std::get<1>(cur_remained_ret);
            // if found, break the loop and find the next goal
            if (goals_extracted_in_cur_layer) {
              break;
            }

            // if not found, remove the action from the extraction map
            extraction_layers[cur_layer].erase(action_node->get_action());

            // add the goal back to the remained goals set
            remained_goals.insert(goal);

            // remove the preconditions from the prev_goals
            for (const auto& precond : action_node->precond_facts_) {
              prev_goals.erase(precond);
            }

#ifdef OUTPUT_DEBUG_INFO
            std::cout << "<<< [PAGraph] Action invalid: "
                      << action_node->get_action() << ", for goal: " << goal
                      << " for current layer, remained goals: ";
            for (const auto& g : remained_goals) {
              std::cout << g << " ";
            }
            std::cout << std::endl;
#endif
          }
        }
      }
      // goals in this layer all extracted and valid
      if (goals_extracted_in_cur_layer) {
        break;
      }
    }
  }

  return std::make_tuple(found_extraction, goals_extracted_in_cur_layer);
}

std::tuple<bool, std::vector<ActionLayerMap>> extract_solution(
    const std::vector<std::string>& goals, const PAGraph& pa_graph) {
  bool solved = false;
  std::vector<ActionLayerMap> extraction_layers(pa_graph.layers);
  std::unordered_set<std::string> goals_set(goals.begin(), goals.end());
#ifdef OUTPUT_DEBUG_INFO
  std::cout << "----------------------------------" << std::endl;
  std::cout << "[PAGraph] Start extracting solution from layer No."
            << pa_graph.layers - 1 << std::endl;
#endif
  auto ret = extract_backward_from_layer(
      goals_set, pa_graph, pa_graph.layers - 1, extraction_layers);
  solved = std::get<0>(ret);
  return std::make_tuple(solved, extraction_layers);
}

bool goal_contained_in_state_layer(const std::vector<std::string>& goals,
                                   const StateLayerMap& state_layer) {
  for (const auto& goal : goals) {
    if (state_layer.find(goal) == state_layer.end()) {
#ifdef OUTPUT_DEBUG_INFO
      std::cout << "[PAGraph] Goal not found: " << goal << std::endl;
#endif
      return false;
    }
  }
  return true;
}

bool exist_mutex_in_goal_layer(const std::vector<std::string>& goals,
                               const PAGraph& pa_graph) {
  for (auto goal1 = goals.begin(); goal1 != goals.end(); ++goal1) {
    for (auto goal2 = std::next(goal1); goal2 != goals.end(); ++goal2) {
      if (two_facts_mutex_in_layer(*goal1, *goal2,
                                   pa_graph.state_mutex_layers.back())) {
#ifdef OUTPUT_DEBUG_INFO
        std::cout << "[PAGraph] Mutex found in goal layer: " << *goal1 << " "
                  << *goal2 << std::endl;
#endif
        return true;
      }
    }
  }
  return false;
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

bool two_actions_mutex_in_layer(const std::string& action1,
                                const std::string& action2,
                                const ActionMutexMap& action_mutex_map) {
  bool mutex = false;
  if (action_mutex_map.find(action1) != action_mutex_map.end() &&
      action_mutex_map.find(action2) != action_mutex_map.end()) {
    if (action_mutex_map.at(action1).find(action2) !=
        action_mutex_map.at(action1).end()) {
#ifdef OUTPUT_DEBUG_INFO
      std::cout << "[PAGraph] Mutex actions found: " << action1 << " "
                << action2 << std::endl;
#endif
      mutex = true;
    }
  }
  return mutex;
}

void print_state_layer(const StateLayerMap& state_layer) {
  for (const auto& it : state_layer) {
    std::cout << it.first << " ";
  }
  std::cout << std::endl;
}

void print_action_layer(const ActionLayerMap& action_layer) {
  for (const auto& it : action_layer) {
    std::cout << it.first << " ";
  }
  std::cout << std::endl;
}
}  // namespace cocg
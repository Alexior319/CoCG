#ifndef COCG_CORE_COCG_GRAPHPLAN_H_
#define COCG_CORE_COCG_GRAPHPLAN_H_

#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "cocg_ast/action.h"
#include "cocg_core/types.h"

namespace cocg {
class PGActionNode;
class PGStateNode;

/**
 * @brief The Action node in the planning graph
 */
class PGActionNode {
 public:
  PGActionNode() {
    action_ = {};
    is_noop_ = true;
    before_state_nodes_ = {};
    after_state_nodes_ = {};
  };
  PGActionNode(const cocg_ast::Action& action,
               const std::unordered_set<std::shared_ptr<PGStateNode>>&
                   before_state_nodes = {},
               const std::unordered_set<std::shared_ptr<PGStateNode>>&
                   after_state_nodes = {})
      : action_(action),
        before_state_nodes_(before_state_nodes),
        after_state_nodes_(after_state_nodes) {
    is_noop_ = false;
    parser::pddl::get_facts_string(action.preconditions, precond_facts_);
    parser::pddl::get_facts_string(action.effects, effect_facts_);
  }
  PGActionNode(const PGActionNode& other) {
    action_ = other.action_;
    is_noop_ = other.is_noop_;
    before_state_nodes_ = other.before_state_nodes_;
    after_state_nodes_ = other.after_state_nodes_;
    precond_facts_ = other.precond_facts_;
    effect_facts_ = other.effect_facts_;
  };
  ~PGActionNode() = default;
  bool is_noop_;
  cocg_ast::Action action_;
  std::vector<std::string> precond_facts_;
  std::vector<std::string> effect_facts_;
  std::unordered_set<std::shared_ptr<PGStateNode>> before_state_nodes_;
  std::unordered_set<std::shared_ptr<PGStateNode>> after_state_nodes_;
};

/**
 * @brief The State node in the planning graph
 */
class PGStateNode {
 public:
  PGStateNode(const cocg_ast::Tree& fact,
              const std::unordered_set<std::shared_ptr<PGActionNode>>&
                  before_action_nodes = {},
              const std::unordered_set<std::shared_ptr<PGActionNode>>&
                  after_action_nodes = {})
      : fact_(fact),
        before_action_nodes_(before_action_nodes),
        after_action_nodes_(after_action_nodes) {}
  PGStateNode(const PGStateNode& other) {
    fact_ = other.fact_;
    before_action_nodes_ = other.before_action_nodes_;
    after_action_nodes_ = other.after_action_nodes_;
  };
  ~PGStateNode() = default;
  cocg_ast::Tree fact_;
  std::unordered_set<std::shared_ptr<PGActionNode>> before_action_nodes_;
  std::unordered_set<std::shared_ptr<PGActionNode>> after_action_nodes_;
};

// (pred_name arg1 arg2 ...) or (not (pred_name arg1 arg2...)) -> state node
using StateLayerMap =
    std::unordered_map<std::string, std::shared_ptr<PGStateNode>>;

// Mutex of a state node with other state nodes in the same state layer
// state node -> vector of state nodes
using StateMutexMap = std::unordered_map<std::string, std::vector<std::string>>;

// (action_name arg1 arg2 ...) -> action node
using ActionLayerMap =
    std::unordered_map<std::string, std::shared_ptr<PGActionNode>>;

// Mutex of an action node with other action nodes in the same action layer
// action node -> vector of action nodes
using ActionMutexMap =
    std::unordered_map<std::string, std::vector<std::string>>;

// Proposition Layer(PL)-Action Layer(AL) Graph
// PL-AL-PL...AL-PL
struct PAGraph {
  PAGraph() {
    layers = 0;
    state_layers = {};
    action_layers = {};
    state_mutex_layers = {};
    action_mutex_layers = {};
  };
  // total layers of the graph
  uint32_t layers;
  // Vector of state(proposition) nodes layers
  std::vector<StateLayerMap> state_layers;
  // Vector of action nodes layers
  std::vector<ActionLayerMap> action_layers;
  // Contains the state/fact/literal(proposition) mutex in each state layer
  std::vector<StateMutexMap> state_mutex_layers;
  // Contains the action mutex in each action layer
  std::vector<ActionMutexMap> action_mutex_layers;
};

/**
 * @brief create the init graph that all goal are satisfied and non-mutex in the
 * last layer
 * @param goals vectors of strings that describe the goal states
 * @param pa_graph the graph to create
 * @param actions grounded action set
 */
void create_init_graph(const std::vector<std::string>& goals, PAGraph& pa_graph,
                       const std::vector<cocg_ast::Action>& actions);

/**
 * @brief expand the graph by one (action and state) layer
 * @param pa_graph the graph to create
 * @param actions grounded action set
 */
void create_graph_layer(PAGraph& pa_graph,
                        const std::vector<cocg_ast::Action>& actions);

/**
 * @brief extract the graph to get a satisfiable solution
 * @param goals vectors of strings that describe the goal states
 * @param pa_graph the graph to create
 * @param actions grounded action set
 * @return (solution_is_valid, solution)
 */
std::tuple<bool, std::vector<ActionLayerMap>> extract_solution(
    const std::vector<std::string>& goals, const PAGraph& pa_graph,
    const std::vector<cocg_ast::Action>& actions);

/**
 * @brief check if the StateLayer contains all the goal facts
 * @param goals goals in the format of a vector of strings
 * @param state_layer a state layer contains some state nodes
 */
bool goal_contained_in_state_layer(const std::vector<std::string>& goals,
                                   const StateLayerMap& state_layer);

/**
 * @brief check if the goals in the last layer of the map are mutex
 * @param goals goals in the format of a vector of strings
 * @param pa_graph the graph currently created
 */
bool exist_mutex_in_goal_layer(const std::vector<std::string>& goals,
                               const PAGraph& pa_graph);
}  // namespace cocg

#endif  // COCG_CORE_COCG_GRAPHPLAN_H_

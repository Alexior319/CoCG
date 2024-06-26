#ifndef COCG_CORE_COCG_UTILS_H_
#define COCG_CORE_COCG_UTILS_H_

#include <chrono>
#include <memory>
#include <tuple>

#include "cocg_core/cocg_problem_expert.h"
#include "cocg_core/cocg_problem_expert_utils.h"
#include "cocg_core/cont_plan_node.h"

namespace cocg {

/**
 * @brief record time for each event.
 */
class Timer {
 public:
  Timer(std::string name) : name_(name) {}
  Timer() { name_ = "Default"; }

  void record(std::string description = "") {
    auto time_point = std::chrono::high_resolution_clock::now();
    time_events_.push_back({time_point, description});
  }

  void print_events() {
    auto events_count = time_events_.size();
    double duration, total_duration;
    for (auto i = 0; i < events_count - 1; i++) {
      auto j = i + 1;
      std::chrono::duration<double, std::ratio<1, 1>> event_duration(
          time_events_[j].first - time_events_[i].first);
      std::chrono::duration<double, std::ratio<1, 1>> from_start_duration(
          time_events_[j].first - time_events_[0].first);
      duration = event_duration.count();
      total_duration = from_start_duration.count();
      std::cout << "[" << this->name_ << "] " << time_events_[j].second << ": "
                << duration << "(s), from start(s): " << total_duration
                << std::endl;
    }
  }

 private:
  std::string name_;
  std::vector<
      std::pair<std::chrono::_V2::system_clock::time_point, std::string>>
      time_events_;
};

/**
 * @brief apply the effects of an actuation action to a state
 * @param init_state the initial state, stored in a proble expert
 * @param action the actuation action
 * @param new_state whether to create a new state or modify the input state
 * @return pointer to the goal state, stored in a proble expert
 */
std::shared_ptr<cocg::ProblemExpert> apply_actuation_action(
    std::shared_ptr<cocg::ProblemExpert> init_state,
    const cocg_ast::Action& action, bool new_state = false);

/**
 * @brief get the name and parameters of a grounded action
 * @param action the action to get the name and parameters
 * @return string format: (action_name arg1 ...), for noop: (_noop fact) where
 * fact: (predicate_name arg1 arg2 ...)
 */
std::string get_grounded_action_string(const cocg_ast::Action& action);

/**
 * @brief get the string format of a fact
 * @param fact the fact encoded in a tree
 * @return string format: (predicate_name arg1 arg2 ...) or (not (predicate_name
 * arg1 arg2 ...))
 */
std::string get_fact_string(const cocg_ast::Tree& fact);

/**
 * @brief get the atom of a fact
 * @param fact the fact encoded in a string
 * @return the atom (without 'not') of the fact
 */
std::string get_atom(const std::string& fact);
/**
 * @brief jugde whether two facts are negated
 * @param s1 the first fact
 * @param s2 the second fact
 */
bool negated_facts(const std::string& s1, const std::string& s2);

/**
 * @brief apply the effects of a sensing action to a state
 * @param init_state the initial state stored in a problem expert
 * @param action the sensing action
 * @param sensing_result the result of the sensing action, true or false
 * @param new_state whether to create a new state or modify the input state
 * @return the goal state
 */
std::shared_ptr<cocg::ProblemExpert> apply_sensing_action(
    std::shared_ptr<cocg::ProblemExpert> init_state,
    const cocg_ast::Action& action, bool sensing_result,
    bool new_state = false);

/**
 * @brief split a grounded action into action name and action arguments
 * @param grounded_action the grounded action, format: (action_name, arg1, arg2,
 * ...)
 * @param action_name the action name
 * @param action_args the action arguments
 */
void split_grounded_action(const std::string& grounded_action,
                           std::string& action_name,
                           std::vector<std::string>& action_args);

/**
 * @brief convert a plan node to an AST action
 * @param node the plan node in a contingent plan tree
 * @param domain_expert the domain expert that initiaized by a domain file
 * @return the AST action
 */
cocg_ast::Action::SharedPtr convert_plan_node_to_ast(
    const cocg::ContPlanNode::SharedPtr node,
    const cocg::DomainExpert& domain_expert);

/**
 * @brief output the tree infl
 * @param root root node of the plan tree
 */
void print_cont_plan_tree(ContPlanNode::SharedPtr root);

/**
 * @brief print the action layers
 * @param action_layers the action layers
 * @param num the number of indent to the right
 * @param indent the indent string
 */
void print_action_layers(
    std::vector<std::vector<cocg_ast::Action>> action_layers,
    const int& num = 0, const std::string& indent = "");
}  // namespace cocg

#endif  // COCG_CORE_COCG_UTILS_H_
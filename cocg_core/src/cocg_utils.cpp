#include "cocg_core/cocg_utils.h"

namespace cocg {

std::shared_ptr<cocg::ProblemExpert> apply_actuation_action(
    std::shared_ptr<cocg::ProblemExpert> init_state,
    const cocg_ast::Action& action, bool new_state) {
  std::shared_ptr<cocg::ProblemExpert> goal_state;
  if (new_state) {
    goal_state = std::make_shared<cocg::ProblemExpert>(init_state);
  } else {
    goal_state = init_state;
  }

  std::cout << "Applying actuation action: " << action.name;
  for (auto p : action.parameters) {
    std::cout << " " << p.name << " - " << p.type << "\t";
  }
  std::cout << "\n";

  cocg::apply(action.effects, goal_state);
  return goal_state;
}

std::shared_ptr<cocg::ProblemExpert> apply_sensing_action(
    std::shared_ptr<cocg::ProblemExpert> init_state,
    const cocg_ast::Action& action, bool sensing_result, bool new_state) {
  std::shared_ptr<cocg::ProblemExpert> goal_state;
  if (new_state) {
    goal_state = std::make_shared<cocg::ProblemExpert>(init_state);
  } else {
    goal_state = init_state;
  }

  std::cout << "Applying sensing action: " << action.name;
  for (auto p : action.parameters) {
    std::cout << " " << p.name << " - " << p.type << " ";
  }
  std::cout << "sensing result: " << sensing_result << "\n";

  std::string observe_unknown_str =
      "(unknown " + parser::pddl::toString(action.observe, 1) + ")";
  cocg::Unknown observe_unknown = cocg::Unknown(observe_unknown_str);

  if (sensing_result) {
    goal_state->removeConditionalUnknown(observe_unknown, true);
  } else {
    goal_state->removeConditionalUnknown(observe_unknown, false);
  }

  return goal_state;
}

void split_grounded_action(const std::string& grounded_action,
                           std::string& action_name,
                           std::vector<std::string>& action_args) {
  size_t start_pos = 1;
  size_t end_pos = grounded_action.find(" ");
  if (end_pos != std::string::npos) {
    action_name = grounded_action.substr(start_pos, end_pos - start_pos);
  } else {
    // output error info
    std::cout << "Invalid grounded action format: " << grounded_action << "\n";
    return;
  }

  while (end_pos != std::string::npos &&
         end_pos < grounded_action.length() - 1) {
    start_pos = end_pos + 1;
    end_pos = grounded_action.find(" ", start_pos);
    if (end_pos != std::string::npos) {
      action_args.push_back(
          grounded_action.substr(start_pos, end_pos - start_pos));
    } else {
      action_args.push_back(grounded_action.substr(
          start_pos, grounded_action.length() - start_pos - 1));
    }
  }
}

cocg_ast::Action::SharedPtr convert_plan_node_to_ast(
    const cocg::ContPlanNode::SharedPtr node,
    cocg::DomainExpert& domain_expert) {
  cocg_ast::Action::SharedPtr action;

  std::string action_name;
  std::vector<std::string> action_args;
  // node.item.action: (action_name arg1 arg2...)
  split_grounded_action(node->item.action, action_name, action_args);

  action = domain_expert.getAction(action_name, action_args);

  return action;
}

void print_cont_plan_tree(ContPlanNode::SharedPtr root) {
  if (root == nullptr) return;
  root->print_info();
  print_cont_plan_tree(root->true_node);
  if (root->true_node != root->false_node)
    print_cont_plan_tree(root->false_node);
}
}  // namespace cocg

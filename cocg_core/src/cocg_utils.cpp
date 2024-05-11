#include "cocg_core/cocg_utils.h"

namespace cocg {

CoCGState apply_actuation_action(const CoCGState& init_state,
                                 const cocg_ast::Action& action) {
  CoCGState goal_state = init_state;
  apply(action.effects, goal_state.predicates, goal_state.functions);
  return goal_state;
}

CoCGState apply_sensing_action(const CoCGState& init_state,
                               const cocg_ast::Action& action,
                               bool sensing_result) {
  CoCGState goal_state = init_state;
  auto problem_expert = std::make_shared<cocg::ProblemExpert>();
  evaluate(action.observe, problem_expert, goal_state.predicates,
           goal_state.functions, true, true, 0, sensing_result);
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
      action_args.push_back(grounded_action.substr(start_pos));
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

std::tuple<cocg::CoCGState, std::vector<cocg::ContPlanNode::SharedPtr>,
           cocg::ContPlanNode::SharedPtr>
traverse_contingent_planning_tree(const cocg::CoCGState& init_state,
                                  cocg::ContPlanNode::SharedPtr root,
                                  cocg::DomainExpert& domain_expert) {
  cocg::CoCGState goal_state = init_state;
  std::vector<cocg::ContPlanNode::SharedPtr> mid_nodes;
  cocg::ContPlanNode::SharedPtr last_node = root;
  while (last_node != nullptr) {
    // sensing action or null action node
    if (last_node->true_node != last_node->false_node) break;

    // actuation action
    mid_nodes.push_back(last_node);

    // action ast
    cocg_ast::Action::SharedPtr action_ast =
        convert_plan_node_to_ast(last_node, domain_expert);
    goal_state = apply_actuation_action(goal_state, *action_ast);

    last_node = last_node->true_node;
  }
  return {goal_state, mid_nodes, last_node};
}
}  // namespace cocg

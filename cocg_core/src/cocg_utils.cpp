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

#ifdef OUTPUT_DEBUG_INFO
  std::cout << "Applying actuation action: (" << action.name;
  for (auto p : action.parameters) {
    std::cout << " " << p.name;
  }
  std::cout << ")\n";
#endif

  cocg::apply(action.effects, goal_state);
  return goal_state;
}

std::string get_grounded_action_string(const cocg_ast::Action& action) {
  std::string action_name = "(" + action.name;
  for (auto p : action.parameters) {
    action_name += " " + p.name;
  }
  action_name += ")";
  return action_name;
}

std::string get_fact_string(const cocg_ast::Tree& fact) {
  return parser::pddl::toString(fact);
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

  std::string observe_str = parser::pddl::toString(action.observe, 1);

#ifdef OUTPUT_DEBUG_INFO
  std::cout << "Applying sensing action: (" << action.name;
  for (auto p : action.parameters) {
    std::cout << " " << p.name;
  }
  std::cout << ") sensing: " << observe_str << " result: " << sensing_result
            << "\n";
#endif

  std::string observe_unknown_str = "(unknown " + observe_str + ")";
  cocg::Unknown observe_unknown = cocg::Unknown(observe_unknown_str);

#ifdef OUTPUT_DEBUG_INFO
  std::cout << "Judging observe unknown: "
            << parser::pddl::toString(observe_unknown) << "\n";
#endif

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
    const cocg::DomainExpert& domain_expert) {
  cocg_ast::Action::SharedPtr action;

  std::string action_name;
  std::vector<std::string> action_args;
  // node.item.action: (action_name arg1 arg2...)
  split_grounded_action(node->item.action, action_name, action_args);

  action = domain_expert.getAction(action_name, action_args);

  return action;
}

void print_cont_tree(std::vector<uint32_t>& cont_tree_branch_cnt,
                     const std::string& prefix, ContPlanNode::SharedPtr node,
                     bool is_true_son, int prev_actions = 0,
                     bool prev_is_sensing = false) {
  if (node != nullptr) {
    prev_actions++;

    std::cout << prefix;
    if (prev_is_sensing) {
      std::cout << (is_true_son ? "├─t " : "└─f ");
    } else {
      std::cout << (is_true_son ? "|   " : "    ");
    }
    std::cout << node->item.action;
    std::cout << std::endl;

    if (node->false_node != node->true_node) {
      print_cont_tree(cont_tree_branch_cnt,
                      prefix + (is_true_son ? "│   " : "    "), node->true_node,
                      true, prev_actions, true);
      print_cont_tree(cont_tree_branch_cnt,
                      prefix + (is_true_son ? "│   " : "    "),
                      node->false_node, false, prev_actions, true);
    } else {
      print_cont_tree(cont_tree_branch_cnt, prefix, node->true_node, true,
                      prev_actions, false);
    }
  } else {
    if (prev_is_sensing) {
      std::cout << prefix;
      if (!is_true_son)
        std::cout << "└─f \n";
      else
        std::cout << "└─t \n";
    }
    std::cout << prefix;
    std::cout << "+-->[END] depth of this branch: " << prev_actions - 1
              << std::endl;
    cont_tree_branch_cnt.push_back(prev_actions - 1);
  }
}

void print_cont_plan_tree(ContPlanNode::SharedPtr root) {
  std::cout << "**************************************\n";
  std::vector<uint32_t> cont_tree_branch_cnt;
  print_cont_tree(cont_tree_branch_cnt, "", root, true, 1);
  std::cout << "[Cont] Total exec time for each branch: " << std::endl;
  for (int i = 0; i < cont_tree_branch_cnt.size(); i++) {
    std::cout << "No." << i + 1 << ": " << cont_tree_branch_cnt[i] << std::endl;
  }
  std::cout << "**************************************\n";
}

bool negated_facts(const std::string& s1, const std::string& s2) {
  // check the propositions are the same
  if (get_atom(s1) != get_atom(s2)) return false;
  if (s1.find("(not ") != std::string::npos &&
      s2.find("(not ") == std::string::npos)
    return true;
  if (s1.find("(not ") == std::string::npos &&
      s2.find("(not ") != std::string::npos)
    return true;
  return false;
}

std::string get_atom(const std::string& fact) {
  std::string atom = fact;
  if (atom.find("(not ") != std::string::npos) {
    atom = atom.substr(5, atom.length() - 6);
  }
  return atom;
}

void print_action_layers(
    std::vector<std::vector<cocg_ast::Action>> action_layers, const int& num,
    const std::string& indent) {
  for (int i = 0; i < action_layers.size(); i++) {
    for (int j = 0; j < num; j++) {
      std::cout << indent;
    }
    std::cout << "Layer " << i << ": ";
    for (int j = 0; j < action_layers[i].size(); j++) {
      std::cout << get_grounded_action_string(action_layers[i][j]) << " ";
    }
    std::cout << std::endl;
  }
}
}  // namespace cocg

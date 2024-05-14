
#ifndef COCG_CORE_CONT_PLAN_NODE_H_
#define COCG_CORE_CONT_PLAN_NODE_H_

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "cocg_ast/plan_item.h"

namespace cocg {
/**
 * @brief A node in a contingent plan tree. If `true_node` and `false_node` are
 * equal, then the node is a actuation action. Otherwise, it is a sensing
 * action.
 */
struct ContPlanNode {
  cocg_ast::PlanItem item;
  std::shared_ptr<ContPlanNode> true_node;
  std::shared_ptr<ContPlanNode> false_node;

  using SharedPtr = std::shared_ptr<ContPlanNode>;

  // comparison operators
  bool operator==(const ContPlanNode& other) const {
    if (this->item != other.item) {
      return false;
    }
    if (this->true_node != other.true_node) {
      return false;
    }
    if (this->false_node != other.false_node) {
      return false;
    }
    return true;
  }
  bool operator!=(const ContPlanNode& other) const {
    return !this->operator==(other);
  }
  void print_info() {
    std::cout << item.time << "|" << item.action << ":" << item.duration;
    if (true_node) std::cout << " true son: " << true_node->item.action;
    if (false_node) std::cout << " false son: " << false_node->item.action;
    std::cout << std::endl;
  }
};

}  // namespace cocg
#endif  // COCG_CORE_CONT_PLAN_NODE_H_
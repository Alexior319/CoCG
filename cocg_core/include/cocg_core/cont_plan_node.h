
#ifndef COCG_CORE_CONT_PLAN_NODE_H_
#define COCG_CORE_CONT_PLAN_NODE_H_

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "cocg_ast/plan_item.h"

namespace cocg {
struct ContPlanNode {
  cocg_ast::PlanItem item;
  std::shared_ptr<ContPlanNode> true_node;
  std::shared_ptr<ContPlanNode> false_node;
};
}  // namespace cocg
#endif  // COCG_CORE_CONT_PLAN_NODE_H_
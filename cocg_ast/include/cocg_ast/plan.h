
#ifndef COCG_AST_PLAN_H_
#define COCG_AST_PLAN_H_

#include <memory>
#include <string>
#include <vector>

#include "cocg_ast/plan_item.h"

namespace cocg_ast {
struct Plan {
  std::vector<cocg_ast::PlanItem> items;
  std::vector<int> structure;

  using SharedPtr = std::shared_ptr<Plan>;

  // comparison operators
  bool operator==(const Plan& other) const {
    if (this->items != other.items) {
      return false;
    }
    if (this->structure != other.structure) {
      return false;
    }
    return true;
  }
  bool operator!=(const Plan& other) const { return !this->operator==(other); }
};
}  // namespace cocg_ast

#endif  // COCG_AST_PLAN_H_

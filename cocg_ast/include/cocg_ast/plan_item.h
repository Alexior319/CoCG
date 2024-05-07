
#ifndef COCG_AST_PLAN_ITEM_H_
#define COCG_AST_PLAN_ITEM_H_

#include <memory>
#include <string>
#include <vector>

namespace cocg_ast {
struct PlanItem {
  double time;
  std::string action;
  double duration;

  using SharedPtr = std::shared_ptr<PlanItem>;

  // comparison operators
  bool operator==(const PlanItem& other) const {
    if (this->time != other.time) {
      return false;
    }
    if (this->action != other.action) {
      return false;
    }
    if (this->duration != other.duration) {
      return false;
    }
    return true;
  }
  bool operator!=(const PlanItem& other) const {
    return !this->operator==(other);
  }
};
}  // namespace cocg_ast

#endif  // COCG_AST_PLAN_ITEM_H_


#ifndef COCG_AST_DURATIVE_ACTION_H_
#define COCG_AST_DURATIVE_ACTION_H_

#include <memory>
#include <string>
#include <vector>

#include "cocg_ast/param.h"
#include "cocg_ast/tree.h"

namespace cocg_ast {
struct DurativeAction {
  std::string name;
  std::vector<cocg_ast::Param> parameters;

  cocg_ast::Tree at_start_requirements;
  cocg_ast::Tree over_all_requirements;
  cocg_ast::Tree at_end_requirements;
  cocg_ast::Tree at_start_effects;
  cocg_ast::Tree at_end_effects;
  cocg_ast::Tree observe;

  using SharedPtr = std::shared_ptr<DurativeAction>;

  // comparison operators
  bool operator==(const DurativeAction& other) const {
    if (this->name != other.name) {
      return false;
    }
    if (this->parameters != other.parameters) {
      return false;
    }
    if (this->at_start_requirements != other.at_start_requirements) {
      return false;
    }
    if (this->over_all_requirements != other.over_all_requirements) {
      return false;
    }
    if (this->at_end_requirements != other.at_end_requirements) {
      return false;
    }
    if (this->name != other.name) {
      return false;
    }
    if (this->at_start_effects != other.at_start_effects) {
      return false;
    }
    if (this->at_end_effects != other.at_end_effects) {
      return false;
    }
    if (this->observe != other.observe) {
      return false;
    }
    return true;
  }
  bool operator!=(const DurativeAction& other) const {
    return !this->operator==(other);
  }
};
}  // namespace cocg_ast

#endif  // COCG_AST_DURATIVE_ACTION_H_


#ifndef COCG_AST_ACTION_H_
#define COCG_AST_ACTION_H_

#include <memory>
#include <string>
#include <vector>

#include "cocg_ast/param.h"
#include "cocg_ast/tree.h"

namespace cocg_ast {
struct Action {
  std::string name;
  std::vector<cocg_ast::Param> parameters;
  cocg_ast::Tree preconditions;
  cocg_ast::Tree effects;
  cocg_ast::Tree observe;

  using SharedPtr = std::shared_ptr<Action>;

  // comparison operators
  bool operator==(const Action& other) const {
    if (this->name != other.name) {
      return false;
    }
    if (this->parameters != other.parameters) {
      return false;
    }
    if (this->preconditions != other.preconditions) {
      return false;
    }
    if (this->effects != other.effects) {
      return false;
    }
    if (this->observe != other.observe) {
      return false;
    }
    return true;
  }
  bool operator!=(const Action& other) const {
    return !this->operator==(other);
  }
};
}  // namespace cocg_ast

#endif  // COCG_AST_ACTION_H_

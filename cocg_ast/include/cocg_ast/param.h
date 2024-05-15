
#ifndef COCG_AST_PARAM_H_
#define COCG_AST_PARAM_H_

#include <memory>
#include <string>
#include <vector>

namespace cocg_ast {
struct Param {
  std::string name; // grounded parameter
  std::string type; // type parameter
  std::vector<std::string> sub_types;

  using SharedPtr = std::shared_ptr<Param>;

  // comparison operators
  bool operator==(const Param& other) const {
    if (this->name != other.name) {
      return false;
    }
    if (this->type != other.type) {
      return false;
    }
    if (this->sub_types != other.sub_types) {
      return false;
    }
    return true;
  }
  bool operator!=(const Param& other) const { return !this->operator==(other); }
};
}  // namespace cocg_ast

#endif  // COCG_AST_PARAM_H_

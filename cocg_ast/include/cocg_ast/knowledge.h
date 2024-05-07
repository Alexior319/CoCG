
#ifndef COCG_AST_KNOWLEDGE_H_
#define COCG_AST_KNOWLEDGE_H_

#include <memory>
#include <string>
#include <vector>

namespace cocg_ast {
struct Knowledge {
  std::vector<std::string> instances;
  std::vector<std::string> predicates;
  std::vector<std::string> functions;
  std::vector<std::string> conditionals;
  std::string goal;

  using SharedPtr = std::shared_ptr<Knowledge>;

  // comparison operators
  bool operator==(const Knowledge& other) const {
    if (this->instances != other.instances) {
      return false;
    }
    if (this->predicates != other.predicates) {
      return false;
    }
    if (this->functions != other.functions) {
      return false;
    }
    if (this->conditionals != other.conditionals) {
      return false;
    }
    if (this->goal != other.goal) {
      return false;
    }
    return true;
  }
  bool operator!=(const Knowledge& other) const {
    return !this->operator==(other);
  }
};
}  // namespace cocg_ast

#endif  // COCG_AST_KNOWLEDGE_H_

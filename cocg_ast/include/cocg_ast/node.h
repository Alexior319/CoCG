
#ifndef COCG_AST_NODE_H_
#define COCG_AST_NODE_H_

#include <memory>
#include <string>
#include <vector>

#include "cocg_ast/param.h"

namespace cocg_ast {
struct Node {
  // constant declarations
  static constexpr uint8_t NONE = 0u;
  static constexpr uint8_t AND = 1u;
  static constexpr uint8_t OR = 2u;
  static constexpr uint8_t NOT = 3u;
  static constexpr uint8_t ACTION = 4u;
  static constexpr uint8_t PREDICATE = 5u;
  static constexpr uint8_t FUNCTION = 6u;
  static constexpr uint8_t EXPRESSION = 7u;
  static constexpr uint8_t FUNCTION_MODIFIER = 8u;
  static constexpr uint8_t NUMBER = 9u;
  static constexpr uint8_t UNKNOWN = 10u;
  static constexpr uint8_t ONE_OF = 11u;
  static constexpr uint8_t FOR_ALL = 12u;
  static constexpr uint8_t COMP_EQ = 13u;
  static constexpr uint8_t COMP_GE = 14u;
  static constexpr uint8_t COMP_GT = 15u;
  static constexpr uint8_t COMP_LE = 16u;
  static constexpr uint8_t COMP_LT = 17u;
  static constexpr uint8_t ARITH_MULT = 18u;
  static constexpr uint8_t ARITH_DIV = 19u;
  static constexpr uint8_t ARITH_ADD = 20u;
  static constexpr uint8_t ARITH_SUB = 21u;
  static constexpr uint8_t ASSIGN = 22u;
  static constexpr uint8_t INCREASE = 23u;
  static constexpr uint8_t DECREASE = 24u;
  static constexpr uint8_t SCALE_UP = 25u;
  static constexpr uint8_t SCALE_DOWN = 26u;

  uint8_t node_type;
  uint8_t expression_type;
  uint8_t modifier_type;

  uint32_t node_id;
  std::vector<uint32_t> children;

  std::string name;
  std::vector<cocg_ast::Param> parameters;
  double value;
  bool negate;

  using SharedPtr = std::shared_ptr<Node>;

  // comparison operators
  bool operator==(const Node& other) const {
    if (this->node_type != other.node_type) {
      return false;
    }
    if (this->expression_type != other.expression_type) {
      return false;
    }
    if (this->modifier_type != other.modifier_type) {
      return false;
    }
    if (this->node_id != other.node_id) {
      return false;
    }
    if (this->children != other.children) {
      return false;
    }
    if (this->name != other.name) {
      return false;
    }
    if (this->parameters != other.parameters) {
      return false;
    }
    if (this->value != other.value) {
      return false;
    }
    if (this->negate != other.negate) {
      return false;
    }
    return true;
  }
  bool operator!=(const Node& other) const { return !this->operator==(other); }
};
}  // namespace cocg_ast

#endif  // COCG_AST_NODE_H_


#ifndef COCG_AST_TREE_H_
#define COCG_AST_TREE_H_

#include <memory>
#include <string>
#include <vector>

#include "cocg_ast/node.h"

namespace cocg_ast {
struct Tree {
  std::vector<cocg_ast::Node> nodes;
  Tree() { nodes = {}; };
  Tree(const Node& node) {
    nodes = {};
    nodes.push_back(node);
  }
  Tree(const Tree& tree) { nodes = tree.nodes; }

  using SharedPtr = std::shared_ptr<Tree>;

  // comparison operators
  bool operator==(const Tree& other) const {
    if (this->nodes != other.nodes) {
      return false;
    }
    return true;
  }
  bool operator!=(const Tree& other) const { return !this->operator==(other); }
};
}  // namespace cocg_ast

#endif  // COCG_AST_TREE_H_

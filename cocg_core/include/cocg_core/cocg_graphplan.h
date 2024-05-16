#ifndef COCG_CORE_COCG_GRAPHPLAN_H_
#define COCG_CORE_COCG_GRAPHPLAN_H_

#include <chrono>
#include <iostream>
#include <memory>
#include <string>

#include "cocg_ast/action.h"
#include "cocg_core/types.h"

namespace cocg {
class PGActionNode;
class PGStateNode;

/**
 * @brief The Action node in the planning graph
 */
class PGActionNode {
 public:
  PGActionNode(
      const cocg_ast::Action& action,
      const std::vector<std::shared_ptr<PGStateNode>>& before_state_nodes = {},
      const std::vector<std::shared_ptr<PGStateNode>>& after_state_nodes = {})
      : action_(action),
        before_state_nodes_(before_state_nodes),
        after_state_nodes_(after_state_nodes) {
    layer_num_ = 0;
  }
  PGActionNode(const PGActionNode& other) {
    action_ = other.action_;
    before_state_nodes_ = other.before_state_nodes_;
    after_state_nodes_ = other.after_state_nodes_;
    layer_num_ = other.layer_num_;
  };
  ~PGActionNode() = default;
  int layer_num_;
  cocg_ast::Action action_;
  std::vector<std::shared_ptr<PGStateNode>> before_state_nodes_;
  std::vector<std::shared_ptr<PGStateNode>> after_state_nodes_;
};

/**
 * @brief The State node in the planning graph
 */
class PGStateNode {
 public:
  PGStateNode(
      const cocg_ast::Tree& fact,
      const std::vector<std::shared_ptr<PGActionNode>>& before_action_nodes =
          {},
      const std::vector<std::shared_ptr<PGActionNode>>& after_action_nodes = {})
      : fact_(fact),
        before_action_nodes_(before_action_nodes),
        after_action_nodes_(after_action_nodes) {
    layer_num_ = 0;
  }
  PGStateNode(const PGStateNode& other) {
    fact_ = other.fact_;
    before_action_nodes_ = other.before_action_nodes_;
    after_action_nodes_ = other.after_action_nodes_;
    layer_num_ = other.layer_num_;
  };
  ~PGStateNode() = default;
  int layer_num_;
  cocg_ast::Tree fact_;
  std::vector<std::shared_ptr<PGActionNode>> before_action_nodes_;
  std::vector<std::shared_ptr<PGActionNode>> after_action_nodes_;
};
}  // namespace cocg

#endif  // COCG_CORE_COCG_GRAPHPLAN_H_

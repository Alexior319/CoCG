
#ifndef COCG_CORE_CONT_PLAN_SOLVER_BASE_H_
#define COCG_CORE_CONT_PLAN_SOLVER_BASE_H_

#include <memory>
#include <optional>
#include <string>

#include "cocg_ast/plan.h"

namespace cocg {

class ContPlanSolverBase {
 public:
  using Ptr = std::shared_ptr<cocg::ContPlanSolverBase>;
  ContPlanSolverBase(){};
  virtual ~ContPlanSolverBase(){};

  /**
   * @brief get a contingent plan ast
   * @param domain the domain content
   * @param problem the problem content
   * @param node_namespace the namespace of the node
   * @return a contingent plan ast
   */
  virtual std::optional<cocg_ast::Plan> get_cocg_cont_plan_ast(
      const std::string& domain, const std::string& problem,
      const std::string& node_namespace = "") = 0;
};

}  // namespace cocg

#endif  // COCG_CORE_CONT_PLAN_SOLVER_BASE_H_
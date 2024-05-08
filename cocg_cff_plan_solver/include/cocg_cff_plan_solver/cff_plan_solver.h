
#ifndef COCG_CFF_PLAN_SOLVER_H_
#define COCG_CFF_PLAN_SOLVER_H_

#include <memory>
#include <optional>
#include <string>

#include "cocg_core/cont_plan_solver_base.h"

namespace cocg {

class CFFPlanSolver : public ContPlanSolverBase {
 public:
  CFFPlanSolver(){};
  ~CFFPlanSolver() override{};

  std::optional<cocg_ast::Plan> get_cocg_cont_plan_ast(
      const std::string& domain, const std::string& problem,
      const std::string& node_namespace = "") override;

 private:
  std::string cff_path_;
};

}  // namespace cocg

#endif  // COCG_CFF_PLAN_SOLVER_H_
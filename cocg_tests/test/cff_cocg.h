#include <chrono>
#include <filesystem>
#include <memory>
#include <ratio>
#include <string>
#include <vector>

#include "cocg_ast/tree.h"
#include "cocg_cff_plan_solver/cff_plan_solver.h"
#include "cocg_core/cocg_core.h"

/**
 * @brief main function of cff + cocg
 * @param domain_path path of the domain file
 * @param problem_path path of the problem file
 */
void cff_cocg(const std::string& domain_path, const std::string& problem_path);

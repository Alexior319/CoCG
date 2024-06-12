
#ifndef COCG_CORE_UTILS_H_
#define COCG_CORE_UTILS_H_

#include <memory>
#include <queue>
#include <string>
#include <vector>

#include "cocg_ast/plan.h"
#include "cocg_ast/plan_item.h"
#include "cont_plan_node.h"

namespace cocg {

std::vector<std::string> tokenize(const std::string& string,
                                  const std::string& delim);

/**
 * @brief get a substring without empty lines
 *
 * @param string original string
 * @param init_pos first character in the original string
 * @param end_pos last character in the original string
 * @return a substring without empty lines
 */
std::string substr_without_empty_lines(std::string string, std::size_t init_pos,
                                       std::size_t end_pos);

/**
 * @brief remove the comments on a pddl string
 *
 * @param pddl a pddl string
 * @return a pddl string without comments
 */
std::string remove_comments(const std::string& pddl);

/**
 * @brief encode a contingent plan node tree to a ast plan
 * @param root the root of the contingent plan node tree
 * @return a ast plan
 */
cocg_ast::Plan encode_plan(const std::shared_ptr<ContPlanNode>& root);

/**
 * @brief decode a ast plan to a contingent plan node tree
 * @param plan a ast plan
 * @return the root of the contingent plan node tree
 */
std::shared_ptr<ContPlanNode> decode_plan(const cocg_ast::Plan& plan);

namespace internal {
void encode_plan(const std::shared_ptr<ContPlanNode>& root,
                 std::vector<int>& struc,
                 std::vector<cocg_ast::PlanItem>& data);

std::shared_ptr<ContPlanNode> decode_plan(
    std::vector<int>& struc, std::vector<cocg_ast::PlanItem>& data);

std::shared_ptr<ContPlanNode> decode_plan(
    std::vector<int>& struc, std::vector<cocg_ast::PlanItem>& data);

std::shared_ptr<ContPlanNode> decode_plan(std::queue<int>& struc,
                                          std::queue<cocg_ast::PlanItem>& data);
}  // namespace internal
}  // namespace cocg

#endif  // COCG_CORE_UTILS_H_

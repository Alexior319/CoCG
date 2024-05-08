
#ifndef COCG_CORE_PROBLEM_EXPERT_UTILS_H_
#define COCG_CORE_PROBLEM_EXPERT_UTILS_H_

#include <map>
#include <memory>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "cocg_ast/tree.h"
#include "cocg_core/cocg_domain_expert.h"
#include "cocg_core/cocg_problem_expert.h"

namespace cocg {

/// Evaluate a PDDL expression represented as a tree.
/**
 * \param[in] node The root node of the PDDL expression.
 * \param[in] problem_client The problem expert client.
 * \param[in] predicates Current predicates state.
 * \param[in] functions Current functions state.
 * \param[in] apply Apply result to problem expert or state.
 * \param[in] use_state Use state representation or problem client.
 * \param[in] negate Invert the truth value.
 * \return result <- tuple(bool, bool, double)
 *         result(0) true if success
 *         result(1) truth value of boolen expression
 *         result(2) value of numeric expression
 */
std::tuple<bool, bool, double> evaluate(
    const cocg_ast::Tree& tree,
    std::shared_ptr<cocg::ProblemExpert> problem_client,
    std::vector<cocg::Predicate>& predicates,
    std::vector<cocg::Function>& functions, bool apply = false,
    bool use_state = false, uint8_t node_id = 0, bool negate = false);

std::tuple<bool, bool, double> evaluate(
    const cocg_ast::Tree& tree,
    std::shared_ptr<cocg::ProblemExpert> problem_client, bool apply = false,
    uint32_t node_id = 0);

std::tuple<bool, bool, double> evaluate(
    const cocg_ast::Tree& tree, std::vector<cocg::Predicate>& predicates,
    std::vector<cocg::Function>& functions, bool apply = false,
    uint32_t node_id = 0);

/// Check a PDDL expression represented as a tree.
/**
 * \param[in] node The root node of the PDDL expression.
 * \param[in] problem_client The problem expert client.
 * \return ret Truth value of the PDDL expression.
 *
 * This function calls the evaluate function.
 */
bool check(const cocg_ast::Tree& tree,
           std::shared_ptr<cocg::ProblemExpert> problem_client,
           uint32_t node_id = 0);

bool check(const cocg_ast::Tree& tree, std::vector<cocg::Predicate>& predicates,
           std::vector<cocg::Function>& functions, uint32_t node_id = 0);

/// Apply a PDDL expression represented as a tree.
/**
 * \param[in] node The root node of the PDDL expression.
 * \param[in] problem_client The problem expert client.
 * \return success Indicates whether the execution was successful.
 *
 * This function calls the evaluate function.
 */
bool apply(const cocg_ast::Tree& tree,
           std::shared_ptr<cocg::ProblemExpert> problem_client,
           uint32_t node_id = 0);

bool apply(const cocg_ast::Tree& tree, std::vector<cocg::Predicate>& predicates,
           std::vector<cocg::Function>& functions, uint32_t node_id = 0);

/// Parse the action expression and time (optional) from an input string.
/**
 * \param[in] input The input string.
 * \return result <- pair(string, int)
 *         result(0) The action expression.
 *         result(1) The action start time.
 *
 * The input string can have either of the following formats.
 *   "(<name> <param_1> ... <param_n>)"
 *   "(<name> <param_1> ... <param_n>):<time>"
 * The output action expression will have the following format.
 *   "<name> <param_1> ... <param_n>"
 */
std::pair<std::string, int> parse_action(const std::string& input);

/// Parse the action expression from an input string.
/**
 * \param[in] input The input string.
 * \return The action expression.
 *
 * The input string can have either of the following formats.
 *   "(<name> <param_1> ... <param_n>)"
 *   "(<name> <param_1> ... <param_n>):<time>"
 */
std::string get_action_expression(const std::string& input);

/// Parse the action time from an input string.
/**
 * \param[in] input The input string.
 * \return The action start time.
 *
 * The input string can have either of the following formats.
 *   "(<name> <param_1> ... <param_n>)"
 *   "(<name> <param_1> ... <param_n>):<time>"
 */
int get_action_time(const std::string& input);

/// Parse the action name from an input string.
/**
 * \param[in] input The input string.
 * \return The name of the action.
 *
 * The input string can have either of the following formats.
 *   "(<name> <param_1> ... <param_n>)"
 *   "(<name> <param_1> ... <param_n>):<time>"
 */
std::string get_action_name(const std::string& input);

/// Parse the action parameter names from an input string.
/**
 * \param[in] input The input string.
 * \return A list of the action parameter names.
 *
 * The input string can have either of the following formats.
 *   "(<name> <param_1> ... <param_n>)"
 *   "(<name> <param_1> ... <param_n>):<time>"
 */
std::vector<std::string> get_action_params(const std::string& action_expr);

}  // namespace cocg

#endif  // COCG_CORE_PROBLEM_EXPERT_UTILS_H_

#ifndef COCG_PDDL_PARSER__UTILS_H_
#define COCG_PDDL_PARSER__UTILS_H_

#include "cocg_ast/action.h"
#include "cocg_ast/durative_action.h"
#include "cocg_ast/node.h"
#include "cocg_ast/param.h"
#include "cocg_ast/tree.h"

namespace parser {
namespace pddl {

/// Removes newlines, duplicated spaces, tabs and spaces from parenthesis
/**
 * \param[in] expr The expression to be reduced
 * \return The reduced expression
 */
std::string getReducedString(const std::string& expr);

/// Returns node type corresponding to the string input
/**
 * \param[in] input The input string
 * \return The node type
 */
uint8_t getNodeType(const std::string& expr,
                    uint8_t def = cocg_ast::Node::NONE);

/// Returns expression type and start position of an expression in a string
/**
 * \param[in] input The input string
 * \return result <- tuple(uint8_t, int)
 *         result(0) The expression type
 *         result(1) The start position of the first occurrence
 */
std::tuple<uint8_t, int> getExpr(const std::string& input);

/// Returns expression type corresponding to the string input
/**
 * \param[in] input The input string
 * \return The expression type
 */
uint8_t getExprType(const std::string& input);

/// Returns function modifier type and start position of a function modifier in
/// a string
/**
 * \param[in] input The input string
 * \return result <- tuple(uint8_t, int)
 *         result(0) The function modifier type
 *         result(1) The start position of the first occurrence
 */
std::tuple<uint8_t, int> getFunMod(const std::string& input);

/// Returns function modifier type corresponding to the string input
/**
 * \param[in] input The input string
 * \return The function modifier type
 */
uint8_t getFunModType(const std::string& input);

int getParenthesis(const std::string& wexpr, int start);

std::vector<std::string> getSubExpr(const std::string& expr);

std::string nameActionsToString(const std::shared_ptr<cocg_ast::Action> action);

std::string nameActionsToString(
    const std::shared_ptr<cocg_ast::DurativeAction> action);

std::string toString(const cocg_ast::Tree& tree, uint32_t node_id = 0,
                     bool negate = false);

std::string toString(const cocg_ast::Node& node);

std::string toStringPredicate(const cocg_ast::Tree& tree, uint32_t node_id,
                              bool negate);

std::string toStringFunction(const cocg_ast::Tree& tree, uint32_t node_id,
                             bool negate);

std::string toStringNumber(const cocg_ast::Tree& tree, uint32_t node_id,
                           bool negate);

std::string toStringAnd(const cocg_ast::Tree& tree, uint32_t node_id,
                        bool negate);

std::string toStringOr(const cocg_ast::Tree& tree, uint32_t node_id,
                       bool negate);

std::string toStringUnknown(const cocg_ast::Tree& tree, uint32_t node_id);

std::string toStringOneOf(const cocg_ast::Tree& tree, uint32_t node_id);

std::string toStringNot(const cocg_ast::Tree& tree, uint32_t node_id,
                        bool negate);

std::string toStringExpression(const cocg_ast::Tree& tree, uint32_t node_id,
                               bool negate);

std::string toStringFunctionModifier(const cocg_ast::Tree& tree,
                                     uint32_t node_id, bool negate);

/// This function creates a complete tree.
/**
 * This function recursivelly extracts the logic expressions and predicates from
 * the expression.
 *
 * \param[in] tree The tree object to be created
 * \param[in] expr A string containing predicates and logic operators
 * \param[in] construct A string containing the associated PDDL constructs
 * \return A smart pointer to the node created
 */
cocg_ast::Node::SharedPtr fromString(cocg_ast::Tree& tree,
                                     const std::string& expr,
                                     bool negate = false,
                                     uint8_t parent = cocg_ast::Node::NONE);

cocg_ast::Tree fromString(const std::string& expr, bool negate = false,
                          uint8_t parent = cocg_ast::Node::NONE);

cocg_ast::Node fromStringPredicate(const std::string& predicate);

cocg_ast::Node fromStringFunction(const std::string& function);

cocg_ast::Param fromStringParam(const std::string& name,
                                const std::string& type = {});

cocg_ast::Tree fromPredicates(const std::vector<std::string>& preds);

cocg_ast::Tree::SharedPtr fromSubtree(const cocg_ast::Tree& subtree,
                                      uint8_t node_type);

cocg_ast::Tree::SharedPtr fromSubtrees(
    const std::vector<cocg_ast::Tree>& subtrees, uint8_t node_type);

std::vector<uint32_t> getSubtreeIds(const cocg_ast::Tree& tree);

std::vector<cocg_ast::Tree> getSubtrees(const cocg_ast::Tree& tree);

void getSubtreeChildren(cocg_ast::Tree& subtree, const cocg_ast::Tree& tree,
                        uint32_t tree_parent, uint32_t subtree_parent);

void getPredicates(std::vector<cocg_ast::Node>& predicates,
                   const cocg_ast::Tree& tree, uint32_t node_id = 0,
                   bool negate = false);

void getFunctions(std::vector<cocg_ast::Node>& functions,
                  const cocg_ast::Tree& tree, uint32_t node_id = 0,
                  bool negate = false);

bool checkTreeEquality(const cocg_ast::Tree& first,
                       const cocg_ast::Tree& second);

bool checkNodeEquality(const cocg_ast::Node& first,
                       const cocg_ast::Node& second);

bool checkParamEquality(const cocg_ast::Param& first,
                        const cocg_ast::Param& second);

bool empty(const cocg_ast::Tree& tree);

}  // namespace pddl
}  // namespace parser

#endif  // COCG_PDDL_PARSER__UTILS_H_

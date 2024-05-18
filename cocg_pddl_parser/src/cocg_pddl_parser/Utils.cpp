
#include "cocg_pddl_parser/Utils.h"

#include <functional>
#include <iostream>
#include <limits>
#include <regex>
#include <set>

namespace parser {
namespace pddl {

std::string getReducedString(const std::string& expr) {
  std::regex nts_chars("[\n\t]*", std::regex_constants::ECMAScript);
  std::string ret = std::regex_replace(expr, nts_chars, "");
  std::regex open_paren("\\( ", std::regex_constants::ECMAScript);
  ret = std::regex_replace(ret, open_paren, "(");
  std::regex close_paren(" \\)", std::regex_constants::ECMAScript);
  ret = std::regex_replace(ret, close_paren, ")");
  return ret;
}

uint8_t getNodeType(const std::string& expr, uint8_t default_node_type) {
  auto node_type = default_node_type;

  std::smatch match;
  int first = std::numeric_limits<int>::max();

  if (std::regex_search(expr, match, std::regex("\\(\\s*and[ (]"))) {
    if (static_cast<int>(match.position()) < first) {
      first = static_cast<int>(match.position());
      node_type = cocg_ast::Node::AND;
    }
  }

  if (std::regex_search(expr, match, std::regex("\\(\\s*or[ (]"))) {
    if (static_cast<int>(match.position()) < first) {
      first = static_cast<int>(match.position());
      node_type = cocg_ast::Node::OR;
    }
  }

  if (std::regex_search(expr, match, std::regex("\\(\\s*not[ (]"))) {
    if (static_cast<int>(match.position()) < first) {
      first = static_cast<int>(match.position());
      node_type = cocg_ast::Node::NOT;
    }
  }

  if (std::regex_search(expr, match, std::regex("\\(\\s*unknown[ (]"))) {
    if (static_cast<int>(match.position()) < first) {
      first = static_cast<int>(match.position());
      node_type = cocg_ast::Node::UNKNOWN;
    }
  }

  if (std::regex_search(expr, match, std::regex("\\(\\s*oneof[ (]"))) {
    if (static_cast<int>(match.position()) < first) {
      first = static_cast<int>(match.position());
      node_type = cocg_ast::Node::ONE_OF;
    }
  }

  std::tuple<uint8_t, int> modifier_search_result = getFunMod(expr);
  if (std::get<0>(modifier_search_result) != cocg_ast::Node::NONE) {
    if (std::get<1>(modifier_search_result) < first) {
      first = std::get<1>(modifier_search_result);
      node_type = cocg_ast::Node::FUNCTION_MODIFIER;
    }
  }

  std::string wexpr = expr;
  while (wexpr.size() > 0) {
    std::regex num_regexp("[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)");

    if (std::regex_search(wexpr, match, num_regexp)) {
      bool valid_number = true;

      // Ignore integer characters within a parameter or construct name.
      // A valid number can only be preceded by a space or a left parenthesis.
      if (match.position() > 0) {
        valid_number = false;
        if (isspace(wexpr.at(match.position() - 1))) {
          valid_number = true;
        } else if (wexpr.at(match.position() - 1) == '(') {
          valid_number = true;
        }
      }

      if (valid_number) {
        if (static_cast<int>(match.position()) < first) {
          first = static_cast<int>(match.position());
          node_type = cocg_ast::Node::NUMBER;
        }
        break;
      } else {
        wexpr.erase(wexpr.begin(),
                    wexpr.begin() + match.position() + match.length());
      }
    } else {
      break;
    }
  }

  // The number search must precede the expression search in order to
  // differentiate between an addition or subtraction expression and a number
  // with a "+" or "-" prefix.
  std::tuple<uint8_t, int> expression_search_result = getExpr(expr);
  if (std::get<0>(expression_search_result) != cocg_ast::Node::NONE) {
    if (std::get<1>(expression_search_result) < first) {
      first = std::get<1>(expression_search_result);
      node_type = cocg_ast::Node::EXPRESSION;
    }
  }

  return node_type;
}

std::tuple<uint8_t, int> getExpr(const std::string& input) {
  auto expr_type = cocg_ast::Node::NONE;

  std::smatch match;
  int first = std::numeric_limits<int>::max();

  if (std::regex_search(input, match, std::regex(">="))) {
    if (static_cast<int>(match.position()) < first) {
      first = static_cast<int>(match.position());
      expr_type = cocg_ast::Node::COMP_GE;
    }
  }

  if (std::regex_search(input, match, std::regex(">"))) {
    if (static_cast<int>(match.position()) < first) {
      first = static_cast<int>(match.position());
      expr_type = cocg_ast::Node::COMP_GT;
    }
  }

  if (std::regex_search(input, match, std::regex("<="))) {
    if (static_cast<int>(match.position()) < first) {
      first = static_cast<int>(match.position());
      expr_type = cocg_ast::Node::COMP_LE;
    }
  }

  if (std::regex_search(input, match, std::regex("<"))) {
    if (static_cast<int>(match.position()) < first) {
      first = static_cast<int>(match.position());
      expr_type = cocg_ast::Node::COMP_LT;
    }
  }

  if (std::regex_search(input, match, std::regex("="))) {
    if (static_cast<int>(match.position()) < first) {
      first = static_cast<int>(match.position());
      expr_type = cocg_ast::Node::COMP_EQ;
    }
  }

  if (std::regex_search(input, match, std::regex("\\*"))) {
    if (static_cast<int>(match.position()) < first) {
      first = static_cast<int>(match.position());
      expr_type = cocg_ast::Node::ARITH_MULT;
    }
  }

  if (std::regex_search(input, match, std::regex("\\/"))) {
    if (static_cast<int>(match.position()) < first) {
      first = static_cast<int>(match.position());
      expr_type = cocg_ast::Node::ARITH_DIV;
    }
  }

  if (std::regex_search(input, match, std::regex("\\+"))) {
    if (static_cast<int>(match.position()) < first) {
      first = static_cast<int>(match.position());
      expr_type = cocg_ast::Node::ARITH_ADD;
    }
  }

  if (std::regex_search(input, match, std::regex("\\-"))) {
    if (static_cast<int>(match.position()) < first) {
      first = static_cast<int>(match.position());
      expr_type = cocg_ast::Node::ARITH_SUB;
    }
  }

  return std::make_tuple(expr_type, first);
}

uint8_t getExprType(const std::string& input) {
  std::tuple<uint8_t, int> result = getExpr(input);
  return std::get<0>(result);
}

std::tuple<uint8_t, int> getFunMod(const std::string& input) {
  auto fun_mod_type = cocg_ast::Node::NONE;

  std::smatch match;
  int first = std::numeric_limits<int>::max();

  if (std::regex_search(input, match, std::regex("assign"))) {
    if (static_cast<int>(match.position()) < first) {
      first = static_cast<int>(match.position());
      fun_mod_type = cocg_ast::Node::ASSIGN;
    }
  }

  if (std::regex_search(input, match, std::regex("increase"))) {
    if (static_cast<int>(match.position()) < first) {
      first = static_cast<int>(match.position());
      fun_mod_type = cocg_ast::Node::INCREASE;
    }
  }

  if (std::regex_search(input, match, std::regex("decrease"))) {
    if (static_cast<int>(match.position()) < first) {
      first = static_cast<int>(match.position());
      fun_mod_type = cocg_ast::Node::DECREASE;
    }
  }

  if (std::regex_search(input, match, std::regex("scale-up"))) {
    if (static_cast<int>(match.position()) < first) {
      first = static_cast<int>(match.position());
      fun_mod_type = cocg_ast::Node::SCALE_UP;
    }
  }

  if (std::regex_search(input, match, std::regex("scale-down"))) {
    if (static_cast<int>(match.position()) < first) {
      first = static_cast<int>(match.position());
      fun_mod_type = cocg_ast::Node::SCALE_DOWN;
    }
  }

  return std::make_tuple(fun_mod_type, first);
}

uint8_t getFunModType(const std::string& input) {
  std::tuple<uint8_t, int> result = getFunMod(input);
  return std::get<0>(result);
}

int getParenthesis(const std::string& wexpr, int start) {
  int it = start + 1;
  int balance = 1;

  while (it < wexpr.size()) {
    if (wexpr[it] == '(') {
      balance++;
    }
    if (wexpr[it] == ')') {
      balance--;
    }

    if (balance == 0) {
      return it;
    }

    it++;
  }

  return it;
}

std::vector<std::string> getSubExpr(const std::string& expr) {
  std::vector<std::string> ret;

  std::string wexpr(expr);

  // Remove first ( and last )
  while (wexpr.back() == ' ') {
    wexpr.pop_back();
  }
  wexpr.pop_back();
  while (wexpr.front() == ' ') {
    wexpr.erase(0, 1);
  }
  wexpr.erase(0, 1);

  while (wexpr.size() > 0) {
    int first = wexpr.find("(");

    std::smatch match;
    std::regex num_regexp("[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)");
    bool found_num = std::regex_search(wexpr, match, num_regexp);

    if (found_num && first != std::string::npos) {
      if (match.position() < first) {
        ret.push_back(wexpr.substr(match.position(), match.length()));
        wexpr.erase(wexpr.begin(),
                    wexpr.begin() + match.position() + match.length());
      } else {
        int last = getParenthesis(wexpr, first);
        ret.push_back(wexpr.substr(first, last - first + 1));
        wexpr.erase(wexpr.begin(), wexpr.begin() + last + 1);
      }
    } else if (found_num) {
      ret.push_back(wexpr.substr(match.position(), match.length()));
      wexpr.erase(wexpr.begin(),
                  wexpr.begin() + match.position() + match.length());
    } else if (first != std::string::npos) {
      int last = getParenthesis(wexpr, first);
      ret.push_back(wexpr.substr(first, last - first + 1));
      wexpr.erase(wexpr.begin(), wexpr.begin() + last + 1);
    } else {
      break;
    }
  }

  return ret;
}

std::string nameActionsToString(
    const std::shared_ptr<cocg_ast::Action> action) {
  std::string ret = action->name;
  for (const auto& param : action->parameters) {
    ret = ret + " " + param.name;
  }
  return ret;
}

std::string nameActionsToString(
    const std::shared_ptr<cocg_ast::DurativeAction> action) {
  std::string ret = action->name;
  for (const auto& param : action->parameters) {
    ret = ret + " " + param.name;
  }
  return ret;
}

std::string toString(const cocg_ast::Tree& tree, uint32_t node_id,
                     bool negate) {
  if (node_id >= tree.nodes.size()) {
    return {};
  }

  std::string ret;
  switch (tree.nodes[node_id].node_type) {
    case cocg_ast::Node::PREDICATE:
      ret = toStringPredicate(tree, node_id, negate);
      break;
    case cocg_ast::Node::FUNCTION:
      ret = toStringFunction(tree, node_id, negate);
      break;
    case cocg_ast::Node::NUMBER:
      ret = toStringNumber(tree, node_id, negate);
      break;
    case cocg_ast::Node::AND:
      ret = toStringAnd(tree, node_id, negate);
      break;
    case cocg_ast::Node::OR:
      ret = toStringOr(tree, node_id, negate);
      break;
    case cocg_ast::Node::NOT:
      ret = toStringNot(tree, node_id, negate);
      break;
    case cocg_ast::Node::UNKNOWN:
      ret = toStringUnknown(tree, node_id);
      break;
    case cocg_ast::Node::ONE_OF:
      ret = toStringOneOf(tree, node_id);
      break;
    case cocg_ast::Node::EXPRESSION:
      ret = toStringExpression(tree, node_id, negate);
      break;
    case cocg_ast::Node::FUNCTION_MODIFIER:
      ret = toStringFunctionModifier(tree, node_id, negate);
      break;
    default:
      std::cerr << "Unsupported node to string conversion" << std::endl;
      break;
  }

  return ret;
}

std::string toString(const cocg_ast::Node& node) {
  if (node.node_type != cocg_ast::Node::PREDICATE &&
      node.node_type != cocg_ast::Node::FUNCTION) {
    std::cerr << "Unsupported node to string conversion" << std::endl;
    return {};
  }

  cocg_ast::Tree tree;
  tree.nodes.push_back(node);

  return toString(tree);
}

std::string toStringPredicate(const cocg_ast::Tree& tree, uint32_t node_id,
                              bool negate) {
  if (node_id >= tree.nodes.size()) {
    return {};
  }

  std::string ret;

  if (negate) {
    ret = "(not (" + tree.nodes[node_id].name;
  } else {
    ret = "(" + tree.nodes[node_id].name;
  }

  for (const auto& param : tree.nodes[node_id].parameters) {
    ret += " " + param.name;
  }

  if (negate) {
    ret += "))";
  } else {
    ret += ")";
  }

  return ret;
}

std::string toStringFunction(const cocg_ast::Tree& tree, uint32_t node_id,
                             bool negate) {
  if (node_id >= tree.nodes.size()) {
    return {};
  }

  std::string ret;

  ret = "(" + tree.nodes[node_id].name;
  for (const auto& param : tree.nodes[node_id].parameters) {
    ret += " " + param.name;
  }

  ret += ")";

  return ret;
}

std::string toStringNumber(const cocg_ast::Tree& tree, uint32_t node_id,
                           bool negate) {
  if (node_id >= tree.nodes.size()) {
    return {};
  }

  return std::to_string(tree.nodes[node_id].value);
}

std::string toStringAnd(const cocg_ast::Tree& tree, uint32_t node_id,
                        bool negate) {
  if (node_id >= tree.nodes.size()) {
    return {};
  }

  if (tree.nodes[node_id].children.empty()) {
    return {};
  }

  std::string ret;

  if (!negate) {
    ret = "(and ";
  } else {
    ret = "(or ";
  }

  for (auto child_id : tree.nodes[node_id].children) {
    ret += toString(tree, child_id, negate);
  }
  ret += ")";

  return ret;
}

std::string toStringOr(const cocg_ast::Tree& tree, uint32_t node_id,
                       bool negate) {
  if (node_id >= tree.nodes.size()) {
    return {};
  }

  if (tree.nodes[node_id].children.empty()) {
    return {};
  }

  std::string ret;

  if (!negate) {
    ret = "(or ";
  } else {
    ret = "(and ";
  }

  for (auto child_id : tree.nodes[node_id].children) {
    ret += toString(tree, child_id, negate);
  }
  ret += ")";

  return ret;
}

std::string toStringOneOf(const cocg_ast::Tree& tree, uint32_t node_id) {
  if (node_id >= tree.nodes.size()) {
    return {};
  }

  if (tree.nodes[node_id].children.empty()) {
    return {};
  }

  std::string ret = "(oneof ";

  for (auto child_id : tree.nodes[node_id].children) {
    ret += toString(tree, child_id);
  }
  ret += ")";

  return ret;
}

std::string toStringUnknown(const cocg_ast::Tree& tree, uint32_t node_id) {
  if (node_id >= tree.nodes.size()) {
    return {};
  }

  if (tree.nodes[node_id].children.empty()) {
    return {};
  }

  std::string ret = "(unknown ";

  for (auto child_id : tree.nodes[node_id].children) {
    ret += toString(tree, child_id);
  }
  ret += ")";

  return ret;
}

std::string toStringNot(const cocg_ast::Tree& tree, uint32_t node_id,
                        bool negate) {
  if (node_id >= tree.nodes.size()) {
    return {};
  }

  if (tree.nodes[node_id].children.empty()) {
    return {};
  }

  return toString(tree, tree.nodes[node_id].children[0], !negate);
}

std::string toStringExpression(const cocg_ast::Tree& tree, uint32_t node_id,
                               bool negate) {
  if (node_id >= tree.nodes.size()) {
    return {};
  }

  if (tree.nodes[node_id].children.size() < 2) {
    return {};
  }

  std::string ret;

  switch (tree.nodes[node_id].expression_type) {
    case cocg_ast::Node::COMP_GE:
      ret = "(>= ";
      break;
    case cocg_ast::Node::COMP_GT:
      ret = "(> ";
      break;
    case cocg_ast::Node::COMP_LE:
      ret = "(<= ";
      break;
    case cocg_ast::Node::COMP_LT:
      ret = "(< ";
      break;
    case cocg_ast::Node::COMP_EQ:
      ret = "(= ";
      break;
    case cocg_ast::Node::ARITH_MULT:
      ret = "(* ";
      break;
    case cocg_ast::Node::ARITH_DIV:
      ret = "(/ ";
      break;
    case cocg_ast::Node::ARITH_ADD:
      ret = "(+ ";
      break;
    case cocg_ast::Node::ARITH_SUB:
      ret = "(- ";
      break;
    default:
      break;
  }

  for (auto child_id : tree.nodes[node_id].children) {
    ret += toString(tree, child_id, negate);
  }
  ret += ")";

  return ret;
}

std::string toStringFunctionModifier(const cocg_ast::Tree& tree,
                                     uint32_t node_id, bool negate) {
  if (node_id >= tree.nodes.size()) {
    return {};
  }

  if (tree.nodes[node_id].children.size() < 2) {
    return {};
  }

  std::string ret;

  switch (tree.nodes[node_id].modifier_type) {
    case cocg_ast::Node::ASSIGN:
      ret = "(assign ";
      break;
    case cocg_ast::Node::INCREASE:
      ret = "(increase ";
      break;
    case cocg_ast::Node::DECREASE:
      ret = "(decrease ";
      break;
    case cocg_ast::Node::SCALE_UP:
      ret = "(scale-up ";
      break;
    case cocg_ast::Node::SCALE_DOWN:
      ret = "(scale-down ";
      break;
    default:
      break;
  }

  for (auto child_id : tree.nodes[node_id].children) {
    ret += toString(tree, child_id, negate);
  }
  ret += ")";

  return ret;
}

cocg_ast::Node::SharedPtr fromString(cocg_ast::Tree& tree,
                                     const std::string& expr, bool negate,
                                     uint8_t parent) {
  std::string wexpr = getReducedString(expr);

  auto default_node_type = cocg_ast::Node::NONE;
  switch (parent) {
    case cocg_ast::Node::AND:
      default_node_type = cocg_ast::Node::PREDICATE;
      break;
    case cocg_ast::Node::OR:
      default_node_type = cocg_ast::Node::PREDICATE;
      break;
    case cocg_ast::Node::NOT:
      default_node_type = cocg_ast::Node::PREDICATE;
      break;
    case cocg_ast::Node::ONE_OF:
      default_node_type = cocg_ast::Node::PREDICATE;
      break;
    case cocg_ast::Node::UNKNOWN:
      default_node_type = cocg_ast::Node::PREDICATE;
      break;
    case cocg_ast::Node::EXPRESSION:
      default_node_type = cocg_ast::Node::FUNCTION;
      break;
    case cocg_ast::Node::FUNCTION_MODIFIER:
      default_node_type = cocg_ast::Node::FUNCTION;
      break;
  }
  auto node_type = getNodeType(wexpr, default_node_type);

  if (wexpr == "(and)" || wexpr == "(and )") {
    return nullptr;
  }
  switch (node_type) {
    case cocg_ast::Node::AND: {
      auto node = std::make_shared<cocg_ast::Node>();
      node->node_type = node_type;
      node->node_id = tree.nodes.size();
      node->negate = negate;
      tree.nodes.push_back(*node);

      std::vector<std::string> subexprs = getSubExpr(wexpr);

      for (const auto& e : subexprs) {
        auto child = fromString(tree, e, negate, node_type);
        tree.nodes[node->node_id].children.push_back(child->node_id);
      }

      return node;
    }
    case cocg_ast::Node::OR: {
      auto node = std::make_shared<cocg_ast::Node>();
      node->node_type = node_type;
      node->node_id = tree.nodes.size();
      node->negate = negate;
      tree.nodes.push_back(*node);

      std::vector<std::string> subexprs = getSubExpr(wexpr);

      for (const auto& e : subexprs) {
        auto child = fromString(tree, e, negate, node_type);
        tree.nodes[node->node_id].children.push_back(child->node_id);
      }

      return node;
    }
    case cocg_ast::Node::NOT: {
      auto node = std::make_shared<cocg_ast::Node>();
      node->node_type = node_type;
      node->node_id = tree.nodes.size();
      node->negate = negate;
      tree.nodes.push_back(*node);

      std::vector<std::string> subexprs = getSubExpr(wexpr);

      auto child = fromString(tree, subexprs[0], !negate, node_type);
      tree.nodes[node->node_id].children.push_back(child->node_id);

      return node;
    }
    case cocg_ast::Node::PREDICATE: {
      auto node = std::make_shared<cocg_ast::Node>(fromStringPredicate(wexpr));
      node->node_id = tree.nodes.size();
      node->negate = negate;
      tree.nodes.push_back(*node);

      return node;
    }
    case cocg_ast::Node::FUNCTION: {
      auto node = std::make_shared<cocg_ast::Node>(fromStringFunction(wexpr));
      node->node_id = tree.nodes.size();
      node->negate = negate;
      tree.nodes.push_back(*node);

      return node;
    }
    case cocg_ast::Node::EXPRESSION: {
      auto node = std::make_shared<cocg_ast::Node>();
      node->node_type = node_type;
      node->expression_type = getExprType(wexpr);
      node->node_id = tree.nodes.size();
      node->negate = negate;
      tree.nodes.push_back(*node);

      std::vector<std::string> subexprs = getSubExpr(wexpr);

      for (const auto& e : subexprs) {
        auto child = fromString(tree, e, false, node_type);
        tree.nodes[node->node_id].children.push_back(child->node_id);
      }

      return node;
    }
    case cocg_ast::Node::FUNCTION_MODIFIER: {
      auto node = std::make_shared<cocg_ast::Node>();
      node->node_type = node_type;
      node->modifier_type = getFunModType(wexpr);
      node->node_id = tree.nodes.size();
      node->negate = negate;
      tree.nodes.push_back(*node);

      std::vector<std::string> subexprs = getSubExpr(wexpr);

      for (const auto& e : subexprs) {
        auto child = fromString(tree, e, false, node_type);
        tree.nodes[node->node_id].children.push_back(child->node_id);
      }

      return node;
    }
    case cocg_ast::Node::NUMBER: {
      auto node = std::make_shared<cocg_ast::Node>();
      node->node_type = node_type;
      node->node_id = tree.nodes.size();
      node->value = std::stod(wexpr);
      node->negate = negate;
      tree.nodes.push_back(*node);

      return node;
    }
    case cocg_ast::Node::ONE_OF: {
      auto node = std::make_shared<cocg_ast::Node>();
      node->node_type = node_type;
      node->node_id = tree.nodes.size();
      node->negate = negate;
      tree.nodes.push_back(*node);

      std::vector<std::string> subexprs = getSubExpr(wexpr);

      for (const auto& e : subexprs) {
        auto child = fromString(tree, e, negate, node_type);
        tree.nodes[node->node_id].children.push_back(child->node_id);
      }

      return node;
    }
    case cocg_ast::Node::UNKNOWN: {
      auto node = std::make_shared<cocg_ast::Node>();
      node->node_type = node_type;
      node->node_id = tree.nodes.size();
      node->negate = negate;
      tree.nodes.push_back(*node);

      std::vector<std::string> subexprs = getSubExpr(wexpr);

      for (const auto& e : subexprs) {
        auto child = fromString(tree, e, negate, node_type);
        tree.nodes[node->node_id].children.push_back(child->node_id);
      }

      return node;
    }
    // LCOV_EXCL_START
    default:
      std::cerr << "fromString: Error parsing expresion [" << wexpr << "]"
                << std::endl;
      // LCOV_EXCL_STOP
  }

  return nullptr;
}

cocg_ast::Tree fromString(const std::string& expr, bool negate,
                          uint8_t parent) {
  cocg_ast::Tree tree;
  fromString(tree, expr, negate, parent);
  return tree;
}

cocg_ast::Node fromStringPredicate(const std::string& predicate) {
  cocg_ast::Node ret;
  ret.node_type = cocg_ast::Node::PREDICATE;

  std::vector<std::string> tokens;
  size_t start = 0, end = 0;

  while (end != std::string::npos) {
    end = predicate.find(" ", start);
    tokens.push_back(predicate.substr(
        start, (end == std::string::npos) ? std::string::npos : end - start));
    start = ((end > (std::string::npos - 1)) ? std::string::npos : end + 1);
  }

  tokens[0].erase(0, 1);
  if (tokens[0].at(tokens[0].size() - 1) ==
      ')') {  // NOTICE: this is needed when there are no parameters
    tokens[0].erase(tokens[0].size() - 1, 1);
  }
  ret.name = tokens[0];

  tokens.back().pop_back();

  for (size_t i = 1; i < tokens.size(); i++) {
    cocg_ast::Param param;
    param.name = tokens[i];
    ret.parameters.push_back(param);
  }

  ret.value = 0;

  return ret;
}

cocg_ast::Node fromStringFunction(const std::string& function) {
  cocg_ast::Node ret;
  ret.node_type = cocg_ast::Node::FUNCTION;

  std::regex name_regexp("[a-zA-Z][a-zA-Z0-9_\\-]*");
  std::regex number_regexp("[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)");

  std::smatch match;
  std::string temp = function;

  if (std::regex_search(temp, match, name_regexp)) {
    ret.name = match.str(0);
    temp = match.suffix().str();
  }

  while (std::regex_search(temp, match, name_regexp)) {
    cocg_ast::Param param;
    param.name = match.str(0);
    ret.parameters.push_back(param);
    temp = match.suffix().str();
  }

  if (std::regex_search(temp, match, number_regexp)) {
    ret.value = std::stod(match.str(0));
  }

  return ret;
}

cocg_ast::Param fromStringParam(const std::string& name,
                                const std::string& type) {
  cocg_ast::Param ret;
  ret.name = name;
  ret.type = type;
  return ret;
}

cocg_ast::Tree fromPredicates(const std::vector<std::string>& preds) {
  cocg_ast::Node node;
  node.node_type = cocg_ast::Node::AND;
  node.node_id = 0;
  node.negate = false;

  cocg_ast::Tree tree;
  tree.nodes.push_back(node);

  for (const auto& pred : preds) {
    auto child = fromStringPredicate(pred);
    child.node_id = tree.nodes.size();
    child.negate = false;
    tree.nodes.push_back(child);
    tree.nodes[0].children.push_back(child.node_id);
  }

  return tree;
}

cocg_ast::Tree::SharedPtr fromSubtree(const cocg_ast::Tree& subtree,
                                      uint8_t node_type) {
  std::vector<cocg_ast::Tree> temp;
  temp.push_back(subtree);
  return fromSubtrees(temp, node_type);
}

cocg_ast::Tree::SharedPtr fromSubtrees(
    const std::vector<cocg_ast::Tree>& subtrees, uint8_t node_type) {
  if (node_type != cocg_ast::Node::AND && node_type != cocg_ast::Node::OR &&
      node_type != cocg_ast::Node::NOT) {
    std::cerr << "fromSubtrees: Unsupported root type." << std::endl;
    return nullptr;
  }

  if (node_type == cocg_ast::Node::NOT && subtrees.size() > 1) {
    std::cerr << "fromSubtree: A NOT node can only operate on single subtree."
              << std::endl;
    return nullptr;
  }

  if (subtrees.empty()) {
    std::cerr << "fromSubtree: The subtrees vector is empty." << std::endl;
    return nullptr;
  }

  cocg_ast::Node node;
  node.node_type = node_type;
  node.node_id = 0;
  node.negate = false;

  auto tree = std::make_shared<cocg_ast::Tree>();
  tree->nodes.push_back(node);

  for (unsigned i = 0; i < subtrees.size(); ++i) {
    auto tree_size = tree->nodes.size();
    tree->nodes[0].children.push_back(tree_size);
    tree->nodes.insert(std::end(tree->nodes), std::begin(subtrees[i].nodes),
                       std::end(subtrees[i].nodes));
    for (unsigned j = 0; j < subtrees[i].nodes.size(); ++j) {
      tree->nodes[tree_size + j].node_id += tree_size;
      for (unsigned k = 0; k < subtrees[i].nodes[j].children.size(); ++k) {
        tree->nodes[tree_size + j].children[k] += tree_size;
      }
    }
    if (node_type == cocg_ast::Node::NOT) {
      tree->nodes[1].negate = true;
      break;
    }
  }

  return tree;
}

cocg_ast::Tree getNegativeTreeFromPredicate(const cocg_ast::Node& predicate){
  cocg_ast::Tree ret;
  std::string predicate_str = toString(predicate);
  predicate_str = "(not " + predicate_str + ")";
  ret = fromString(predicate_str);
  return ret;
}

void get_facts_string(const cocg_ast::Tree& tree,
                      std::vector<std::string>& results) {
  std::vector<cocg_ast::Node> predicates;
  parser::pddl::getPredicates(predicates, tree);
  for (const auto& pred : predicates) {
    cocg_ast::Tree temp(pred);
    auto fact = parser::pddl::toString(pred, 0, pred.negate);
    results.push_back(fact);
  }
}

std::vector<uint32_t> getSubtreeIds(const cocg_ast::Tree& tree) {
  if (tree.nodes.empty()) {  // No expression
    return {};
  }

  switch (tree.nodes.front().node_type) {
    case cocg_ast::Node::AND: {
      return tree.nodes.front().children;
    }
    default:
      std::cerr << "getSubtreeIds: Error parsing expresion [" << toString(tree)
                << "]" << std::endl;
  }

  return {};
}

std::vector<cocg_ast::Tree> getSubtrees(const cocg_ast::Tree& tree) {
  std::vector<uint32_t> node_ids = parser::pddl::getSubtreeIds(tree);
  std::vector<cocg_ast::Tree> subtrees;
  for (auto node_id : node_ids) {
    cocg_ast::Tree subtree;
    subtree.nodes.push_back(tree.nodes[node_id]);
    subtree.nodes[0].node_id = 0;
    subtree.nodes[0].children.clear();
    getSubtreeChildren(subtree, tree, node_id, 0);
    subtrees.push_back(subtree);
  }
  return subtrees;
}

void getSubtreeChildren(cocg_ast::Tree& subtree, const cocg_ast::Tree& tree,
                        uint32_t tree_parent, uint32_t subtree_parent) {
  for (auto child_id : tree.nodes[tree_parent].children) {
    auto subtree_size = subtree.nodes.size();
    subtree.nodes[subtree_parent].children.push_back(subtree_size);
    subtree.nodes.push_back(tree.nodes[child_id]);
    subtree.nodes.back().node_id = subtree_size;
    subtree.nodes.back().children.clear();
    getSubtreeChildren(subtree, tree, child_id, subtree_size);
  }
}

void getPredicates(std::vector<cocg_ast::Node>& predicates,
                   const cocg_ast::Tree& tree, uint32_t node_id, bool negate) {
  if (node_id >= tree.nodes.size()) {
    return;
  }

  switch (tree.nodes[node_id].node_type) {
    case cocg_ast::Node::FUNCTION:
    case cocg_ast::Node::EXPRESSION:
    case cocg_ast::Node::FUNCTION_MODIFIER:
    case cocg_ast::Node::NUMBER:
      // These cases have no meaning
      break;
    case cocg_ast::Node::AND:
      for (auto child_id : tree.nodes[node_id].children) {
        getPredicates(predicates, tree, child_id, negate);
      }
      break;
    case cocg_ast::Node::OR:
      for (auto child_id : tree.nodes[node_id].children) {
        getPredicates(predicates, tree, child_id, negate);
      }
      break;
    case cocg_ast::Node::NOT:
      getPredicates(predicates, tree, tree.nodes[node_id].children[0], !negate);
      break;
    case cocg_ast::Node::UNKNOWN:
      for (auto child_id : tree.nodes[node_id].children) {
        getPredicates(predicates, tree, child_id, negate);
      }
      break;
    case cocg_ast::Node::ONE_OF:
      for (auto child_id : tree.nodes[node_id].children) {
        getPredicates(predicates, tree, child_id, negate);
      }
      break;
    case cocg_ast::Node::ACTION:
      for (auto child_id : tree.nodes[node_id].children) {
        getPredicates(predicates, tree, child_id, negate);
      }
      break;
    case cocg_ast::Node::PREDICATE:
      cocg_ast::Node pred = tree.nodes[node_id];
      if (std::find_if(predicates.begin(), predicates.end(),
                       [pred](const cocg_ast::Node& p1) {
                         return p1 == pred;
                       }) == predicates.end()) {
        pred.negate = negate;
        predicates.push_back(pred);
      }
      break;
  }
}

void getFunctions(std::vector<cocg_ast::Node>& functions,
                  const cocg_ast::Tree& tree, uint32_t node_id, bool negate) {
  if (node_id >= tree.nodes.size()) {
    return;
  }

  switch (tree.nodes[node_id].node_type) {
    case cocg_ast::Node::AND:
    case cocg_ast::Node::OR:
    case cocg_ast::Node::NOT:
    case cocg_ast::Node::NUMBER:
      // These cases have no meaning
      break;
    case cocg_ast::Node::ACTION:
      for (auto child_id : tree.nodes[node_id].children) {
        getFunctions(functions, tree, child_id, negate);
      }
      break;
    case cocg_ast::Node::EXPRESSION:
      for (auto child_id : tree.nodes[node_id].children) {
        getFunctions(functions, tree, child_id, negate);
      }
      break;
    case cocg_ast::Node::FUNCTION_MODIFIER:
      for (auto child_id : tree.nodes[node_id].children) {
        getFunctions(functions, tree, child_id, negate);
      }
      break;
    case cocg_ast::Node::FUNCTION:
      cocg_ast::Node func = tree.nodes[node_id];
      if (std::find_if(functions.begin(), functions.end(),
                       std::bind(&checkNodeEquality, std::placeholders::_1,
                                 func)) == functions.end()) {
        func.value = 0.0;
        functions.push_back(func);
      }
      break;
  }
}

bool checkTreeEquality(const cocg_ast::Tree& first,
                       const cocg_ast::Tree& second) {
  if (first.nodes.size() != second.nodes.size()) {
    return false;
  }

  for (unsigned i = 0; i < first.nodes.size(); i++) {
    if (!checkNodeEquality(first.nodes[i], second.nodes[i])) {
      return false;
    }
  }

  return true;
}

bool checkNodeEquality(const cocg_ast::Node& first,
                       const cocg_ast::Node& second) {
  if (first.node_type != second.node_type) {
    return false;
  }

  if (first.node_type == cocg_ast::Node::ACTION ||
      first.node_type == cocg_ast::Node::PREDICATE ||
      first.node_type == cocg_ast::Node::FUNCTION) {
    if (first.name != second.name) {
      return false;
    }
  }

  if (first.node_type == cocg_ast::Node::EXPRESSION) {
    if (first.expression_type != second.expression_type) {
      return false;
    }
  }

  if (first.node_type == cocg_ast::Node::FUNCTION_MODIFIER) {
    if (first.modifier_type != second.modifier_type) {
      return false;
    }
  }

  if (first.node_type == cocg_ast::Node::NUMBER) {
    if (abs(first.value - second.value) > 1e-9) {
      return false;
    }
  }

  if (first.children.size() != second.children.size()) {
    return false;
  }

  if (first.parameters.size() != second.parameters.size()) {
    return false;
  }

  for (unsigned i = 0; i < first.parameters.size(); i++) {
    if (!checkParamEquality(first.parameters[i], second.parameters[i])) {
      return false;
    }
  }

  return true;
}

bool checkParamEquality(const cocg_ast::Param& first,
                        const cocg_ast::Param& second) {
  if (first.name != second.name) {
    return false;
  }

  return true;
}

bool empty(const cocg_ast::Tree& tree) {
  if (tree.nodes.empty()) {
    return true;
  }

  if ((tree.nodes[0].node_type == cocg_ast::Node::AND ||
       tree.nodes[0].node_type == cocg_ast::Node::OR ||
       tree.nodes[0].node_type == cocg_ast::Node::NOT ||
       tree.nodes[0].node_type == cocg_ast::Node::EXPRESSION ||
       tree.nodes[0].node_type == cocg_ast::Node::FUNCTION_MODIFIER) &&
      tree.nodes[0].children.empty()) {
    return true;
  }

  return false;
}

}  // namespace pddl
}  // namespace parser

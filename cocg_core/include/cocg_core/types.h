#ifndef COCG_CORE_TYPES_H_
#define COCG_CORE_TYPES_H_

#include <string>
#include <vector>

#include "cocg_ast/node.h"
#include "cocg_ast/param.h"
#include "cocg_ast/tree.h"
#include "cocg_pddl_parser/Utils.h"

namespace cocg {

class Instance : public cocg_ast::Param {
 public:
  Instance() : cocg_ast::Param() {}
  explicit Instance(const std::string& name, const std::string& type = {})
      : cocg_ast::Param(parser::pddl::fromStringParam(name, type)) {}
  Instance(const cocg_ast::Param& instance)  // NOLINT(runtime/explicit)
      : cocg_ast::Param(instance) {}
};

class Predicate : public cocg_ast::Node {
 public:
  Predicate() : cocg_ast::Node() {}
  explicit Predicate(const std::string& pred)
      : cocg_ast::Node(parser::pddl::fromStringPredicate(pred)) {}
  Predicate(const cocg_ast::Node& pred)  // NOLINT(runtime/explicit)
      : cocg_ast::Node(pred) {}
};

class Function : public cocg_ast::Node {
 public:
  Function() : cocg_ast::Node() {}
  explicit Function(const std::string& func)
      : cocg_ast::Node(parser::pddl::fromStringFunction(func)) {}
  Function(const cocg_ast::Node& func)  // NOLINT(runtime/explicit)
      : cocg_ast::Node(func) {}
};

class Goal : public cocg_ast::Tree {
 public:
  Goal() : cocg_ast::Tree() {}
  explicit Goal(const std::string& goal)
      : cocg_ast::Tree(parser::pddl::fromString(goal)) {}
  Goal(const cocg_ast::Tree& goal)  // NOLINT(runtime/explicit)
      : cocg_ast::Tree(goal) {}
};

class Or : public cocg_ast::Tree {
 public:
  Or() : cocg_ast::Tree() {}
  explicit Or(const std::string& or_condition)
      : cocg_ast::Tree(parser::pddl::fromString(or_condition)) {}
  Or(const cocg_ast::Tree& or_condition)  // NOLINT(runtime/explicit)
      : cocg_ast::Tree(or_condition) {}
};

class OneOf : public cocg_ast::Tree {
 public:
  OneOf() : cocg_ast::Tree() {}
  explicit OneOf(const std::string& condition)
      : cocg_ast::Tree(parser::pddl::fromString(condition)) {}
  OneOf(const cocg_ast::Tree& condition)  // NOLINT(runtime/explicit)
      : cocg_ast::Tree(condition) {}
};

class Unknown : public cocg_ast::Tree {
 public:
  Unknown() : cocg_ast::Tree() {}
  explicit Unknown(const std::string& condition)
      : cocg_ast::Tree(parser::pddl::fromString(condition)) {}
  Unknown(const cocg_ast::Tree& condition)  // NOLINT(runtime/explicit)
      : cocg_ast::Tree(condition) {}
};

template <class toT, class fromT>
std::vector<toT> convertVector(const std::vector<fromT>& in_vector) {
  std::vector<toT> ret;
  for (const auto& item : in_vector) {
    ret.push_back(item);
  }
  return ret;
}

}  // namespace cocg

#endif  // COCG_CORE_TYPES_H_

#ifndef COCG_PDDL_PARSER_GROUNDFUNC_H_
#define COCG_PDDL_PARSER_GROUNDFUNC_H_

#include "cocg_ast/node.h"
#include "cocg_ast/tree.h"
#include "cocg_pddl_parser/TypeGround.h"

namespace parser {
namespace pddl {

template <typename T>
class GroundFunc : public TypeGround {
 public:
  T value;

  GroundFunc() : TypeGround(), value(0) {}

  GroundFunc(Lifted* l, const T& val = T(0)) : TypeGround(l), value(val) {}

  void PDDLPrint(std::ostream& s, unsigned indent,
                 const TokenStruct<std::string>& ts,
                 const Domain& d) const override;

  cocg_ast::Node::SharedPtr getTree(
      cocg_ast::Tree& tree, const Domain& d,
      const std::vector<std::string>& replace = {}) const override;

  void print(std::ostream& stream) const {
    stream << name << params << value << "\n";
  }

  void parse(Stringreader& f, TokenStruct<std::string>& ts, Domain& d);
};

}  // namespace pddl
}  // namespace parser

#endif  // COCG_PDDL_PARSER_GROUNDFUNC_H_
#ifndef COCG_PDDL_PARSER_DERIVED_H_
#define COCG_PDDL_PARSER_DERIVED_H_

#include "cocg_ast/node.h"
#include "cocg_ast/tree.h"
#include "cocg_pddl_parser/Lifted.h"

namespace parser {
namespace pddl {

class Derived : public Lifted {
 public:
  Condition* cond;
  Lifted* lifted;

  Derived() : Lifted(), cond(0), lifted(0) {}

  Derived(const std::string s) : Lifted(s), cond(0), lifted(0) {}

  Derived(const Derived* z, Domain& d);

  void print(std::ostream& stream) const {
    stream << "Derived ";
    ParamCond::print(stream);
    if (cond) {
      cond->print(stream);
    }
  }

  void PDDLPrint(std::ostream& s, unsigned indent,
                 const TokenStruct<std::string>& ts,
                 const Domain& d) const override;

  cocg_ast::Node::SharedPtr getTree(
      cocg_ast::Tree& tree, const Domain& d,
      const std::vector<std::string>& replace = {}) const override;

  void parse(Stringreader& f, TokenStruct<std::string>& ts, Domain& d);

  void addParams(int m, unsigned n) {
    for (unsigned i = 0; i < params.size(); ++i) {
      if (params[i] >= m) {
        params[i] += n;
      }
    }
  }

  Condition* copy(Domain& d) { return new Derived(this, d); }
};

}  // namespace pddl
}  // namespace parser

#endif  // COCG_PDDL_PARSER_DERIVED_H_
#ifndef COCG_PDDL_PARSER_EXISTS_H_
#define COCG_PDDL_PARSER_EXISTS_H_

#include "cocg_ast/node.h"
#include "cocg_ast/tree.h"
#include "cocg_pddl_parser/ParamCond.h"

namespace parser {
namespace pddl {

class Exists : public ParamCond {
 public:
  Condition* cond;

  Exists() : cond(0) {}

  Exists(const Exists* e, Domain& d) : ParamCond(e), cond(0) {
    if (e->cond) {
      cond = e->cond->copy(d);
    }
  }

  ~Exists() {
    if (cond) {
      delete cond;
    }
  }

  void print(std::ostream& s) const {
    s << "Exists" << params << ":\n";
    cond->print(s);
  }

  void PDDLPrint(std::ostream& s, unsigned indent,
                 const TokenStruct<std::string>& ts,
                 const Domain& d) const override;

  cocg_ast::Node::SharedPtr getTree(
      cocg_ast::Tree& tree, const Domain& d,
      const std::vector<std::string>& replace = {}) const override;

  void parse(Stringreader& f, TokenStruct<std::string>& ts, Domain& d);

  void addParams(int m, unsigned n) { cond->addParams(m, n); }

  Condition* copy(Domain& d) { return new Exists(this, d); }
};

}  // namespace pddl
}  // namespace parser

#endif  // COCG_PDDL_PARSER_EXISTS_H_
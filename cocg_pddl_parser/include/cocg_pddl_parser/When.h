#ifndef COCG_PDDL_PARSER_WHEN_H_
#define COCG_PDDL_PARSER_WHEN_H_

#include "cocg_ast/node.h"
#include "cocg_ast/tree.h"
#include "cocg_pddl_parser/Condition.h"

namespace parser {
namespace pddl {

class When : public Condition {
 public:
  Condition *pars, *cond;

  When() : pars(0), cond(0) {}

  When(const When* w, Domain& d) : pars(0), cond(0) {
    if (w->pars) {
      pars = w->pars->copy(d);
    }
    if (w->cond) {
      cond = w->cond->copy(d);
    }
  }

  ~When() {
    if (pars) {
      delete pars;
    }
    if (cond) {
      delete cond;
    }
  }

  void print(std::ostream& s) const {
    s << "when:\n";
    if (pars) {
      pars->print(s);
    }
    if (cond) {
      cond->print(s);
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
    pars->addParams(m, n);
    cond->addParams(m, n);
  }

  Condition* copy(Domain& d) { return new When(this, d); }
};

}  // namespace pddl
}  // namespace parser

#endif  // COCG_PDDL_PARSER_WHEN_H_
#pragma once

#include "cocg_ast/node.h"
#include "cocg_ast/tree.h"
#include "cocg_pddl_parser/ParamCond.h"

namespace parser {
namespace pddl {

class Forall : public ParamCond {
 public:
  Condition* cond;

  Forall() : cond(0) {}

  Forall(const Forall* f, Domain& d) : ParamCond(f), cond(0) {
    if (f->cond) {
      cond = f->cond->copy(d);
    }
  }

  ~Forall() {
    if (cond) {
      delete cond;
    }
  }

  void print(std::ostream& s) const {
    s << "Forall" << params << ":\n";
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

  void addParams(int m, unsigned n) { cond->addParams(m, n); }

  Condition* copy(Domain& d) { return new Forall(this, d); }
};

}  // namespace pddl
}  // namespace parser

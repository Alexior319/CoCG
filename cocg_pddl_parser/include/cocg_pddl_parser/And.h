#ifndef COCG_PDDL_PARSER_AND_H_
#define COCG_PDDL_PARSER_AND_H_

#include "cocg_ast/tree.h"
#include "cocg_ast/node.h"
#include "cocg_pddl_parser/Condition.h"

namespace parser {
namespace pddl {

class And : public Condition {
 public:
  CondVec conds;

  And() = default;

  And(const And* a, Domain& d) {
    for (unsigned i = 0; i < a->conds.size(); ++i) {
      conds.push_back(a->conds[i]->copy(d));
    }
  }

  ~And() {
    for (unsigned i = 0; i < conds.size(); ++i) {
      delete conds[i];
    }
  }

  void print(std::ostream& s) const {
    for (unsigned i = 0; i < conds.size(); ++i) {
      conds[i]->print(s);
    }
  }

  void PDDLPrint(std::ostream& s, unsigned indent,
                 const TokenStruct<std::string>& ts,
                 const Domain& d) const override;

  cocg_ast::Node::SharedPtr getTree(
      cocg_ast::Tree& tree, const Domain& d,
      const std::vector<std::string>& replace = {}) const override;

  void parse(Stringreader& f, TokenStruct<std::string>& ts, Domain& d);

  void add(Condition* cond) { conds.push_back(cond); }

  void addParams(int m, unsigned n) {
    for (unsigned i = 0; i < conds.size(); ++i) {
      conds[i]->addParams(m, n);
    }
  }

  Condition* copy(Domain& d) { return new And(this, d); }
};

}  // namespace pddl
}  // namespace parser

#endif  // COCG_PDDL_PARSER_AND_H_
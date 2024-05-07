#ifndef COCG_PDDL_PARSER_NOT_H_
#define COCG_PDDL_PARSER_NOT_H_

#include "cocg_ast/node.h"
#include "cocg_ast/tree.h"
#include "cocg_pddl_parser/Ground.h"

namespace parser {
namespace pddl {

class Not : public Condition {
 public:
  Ground *cond;

  Not() : cond(0) {}

  Not(Ground *g) : cond(g) {}

  Not(const Not *n, Domain &d) : cond(0) {
    if (n->cond) {
      cond = (Ground *)n->cond->copy(d);
    }
  }

  ~Not() {
    if (cond) {
      delete cond;
    }
  }

  void print(std::ostream &s) const {
    s << "not ";
    if (cond) {
      cond->print(s);
    }
  }

  void PDDLPrint(std::ostream &s, unsigned indent,
                 const TokenStruct<std::string> &ts,
                 const Domain &d) const override;

  cocg_ast::Node::SharedPtr getTree(
      cocg_ast::Tree &tree, const Domain &d,
      const std::vector<std::string> &replace = {}) const override;

  void parse(Stringreader &f, TokenStruct<std::string> &ts, Domain &d);

  void addParams(int m, unsigned n) { cond->addParams(m, n); }

  Condition *copy(Domain &d) { return new Not(this, d); }
};

}  // namespace pddl
}  // namespace parser

#endif  // COCG_PDDL_PARSER_NOT_H_
#ifndef COCG_PDDL_PARSER_OR_H_
#define COCG_PDDL_PARSER_OR_H_

#include "cocg_ast/node.h"
#include "cocg_ast/tree.h"
#include "cocg_pddl_parser/Condition.h"

namespace parser {
namespace pddl {

class Or : public Condition {
 public:
  Condition *first, *second;

  Or() : first(0), second(0) {}

  Or(const Or* o, Domain& d) : first(0), second(0) {
    if (o->first) {
      first = o->first->copy(d);
    }
    if (o->second) {
      second = o->second->copy(d);
    }
  }

  ~Or() {
    if (first) {
      delete first;
    }
    if (second) {
      delete second;
    }
  }

  void print(std::ostream& s) const {
    s << "OR:\n";
    if (first) {
      first->print(s);
    }
    if (second) {
      second->print(s);
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
    first->addParams(m, n);
    second->addParams(m, n);
  }

  Condition* copy(Domain& d) { return new Or(this, d); }
};

}  // namespace pddl
}  // namespace parser

#endif  // COCG_PDDL_PARSER_OR_H_
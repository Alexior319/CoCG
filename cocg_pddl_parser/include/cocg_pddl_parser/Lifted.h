#ifndef COCG_PDDL_PARSER_LIFTED_H_
#define COCG_PDDL_PARSER_LIFTED_H_

#include "cocg_ast/node.h"
#include "cocg_ast/tree.h"
#include "cocg_pddl_parser/ParamCond.h"

namespace parser {
namespace pddl {

class Lifted : public ParamCond {
 public:
  Lifted() {}

  Lifted(const std::string& s) : ParamCond(s) {}

  Lifted(const ParamCond* c) : ParamCond(c) {}

  void PDDLPrint(std::ostream& s, unsigned indent,
                 const TokenStruct<std::string>& ts,
                 const Domain& d) const override;

  cocg_ast::Node::SharedPtr getTree(
      cocg_ast::Tree& tree, const Domain& d,
      const std::vector<std::string>& replace = {}) const override;

  void parse(Stringreader& f, TokenStruct<std::string>& ts, Domain& d);

  void addParams(int m, unsigned n) {}

  Condition* copy(Domain& d) { return new Lifted(this); }
};

typedef std::vector<Lifted*> LiftedVec;

}  // namespace pddl
}  // namespace parser

#endif  // COCG_PDDL_PARSER_LIFTED_H_
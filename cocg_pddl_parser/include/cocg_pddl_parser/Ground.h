#ifndef COCG_PDDL_PARSER_GROUND_H_
#define COCG_PDDL_PARSER_GROUND_H_

#include "cocg_ast/node.h"
#include "cocg_ast/tree.h"
#include "cocg_pddl_parser/Lifted.h"

namespace parser {
namespace pddl {

class Ground : public ParamCond {
 public:
  Lifted* lifted;

  Ground() : ParamCond(), lifted(0) {}

  Ground(const std::string s, const IntVec& p = IntVec())
      : ParamCond(s, p), lifted(0) {}

  Ground(Lifted* l, const IntVec& p = IntVec())
      : ParamCond(l->name, p), lifted(l) {}

  Ground(const Ground* g, Domain& d);

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

  Condition* copy(Domain& d) { return new Ground(this, d); }
};

typedef std::vector<Ground*> GroundVec;

}  // namespace pddl
}  // namespace parser

#endif  // COCG_PDDL_PARSER_GROUND_H_
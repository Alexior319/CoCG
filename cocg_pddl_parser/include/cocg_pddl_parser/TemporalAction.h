#ifndef COCG_PDDL_PARSER_TEMPORALACTION_H_
#define COCG_PDDL_PARSER_TEMPORALACTION_H_

#include "cocg_ast/node.h"
#include "cocg_ast/tree.h"
#include "cocg_pddl_parser/Action.h"
#include "cocg_pddl_parser/And.h"
#include "cocg_pddl_parser/Expression.h"

namespace parser {
namespace pddl {

class Instance;

class TemporalAction : public Action {
 public:
  Expression* durationExpr;
  // pre_s and eff_s inherited from Action
  And *pre_o, *pre_e, *eff_e;

  TemporalAction(const std::string& s)
      : Action(s), durationExpr(0), pre_o(0), pre_e(0), eff_e(0) {}

  ~TemporalAction() {
    if (durationExpr) {
      delete durationExpr;
    }
    if (pre_o) {
      delete pre_o;
    }
    if (pre_e) {
      delete pre_e;
    }
    if (eff_e) {
      delete eff_e;
    }
  }

  void print(std::ostream& s) const {
    s << name << params << "\n";
    s << "Duration: " << durationExpr->info() << "\n";
    s << "Pre_s: " << pre;
    s << "Eff_s: " << eff;
    s << "Pre_o: " << pre_o;
    s << "Pre_e: " << pre_e;
    s << "Eff_e: " << eff_e;
    s << "Observe: " << observe;
  }

  double duration() {
    if (durationExpr) {
      return durationExpr->evaluate();
    } else {
      return 0;
    }
  }

  Expression* parseDuration(Stringreader& f, TokenStruct<std::string>& ts,
                            Domain& d);

  void printCondition(std::ostream& s, const TokenStruct<std::string>& ts,
                      const Domain& d, const std::string& t, And* a) const;

  void PDDLPrint(std::ostream& s, unsigned indent,
                 const TokenStruct<std::string>& ts,
                 const Domain& d) const override;

  cocg_ast::Node::SharedPtr getTree(
      cocg_ast::Tree& tree, const Domain& d,
      const std::vector<std::string>& replace = {}) const override;

  void parseCondition(Stringreader& f, TokenStruct<std::string>& ts, Domain& d,
                      And* a);

  void parse(Stringreader& f, TokenStruct<std::string>& ts, Domain& d);

  GroundVec preconsStart();

  GroundVec preconsOverall();

  GroundVec preconsEnd();

  CondVec endEffects();

  GroundVec addEndEffects();

  GroundVec deleteEndEffects();
};

}  // namespace pddl
}  // namespace parser

#endif  // COCG_PDDL_PARSER_TEMPORALACTION_H_

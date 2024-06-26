#ifndef COCG_PDDL_PARSER_ACTION_H_
#define COCG_PDDL_PARSER_ACTION_H_

#include "cocg_ast/tree.h"
#include "cocg_ast/node.h"
#include "cocg_pddl_parser/Ground.h"

namespace parser {
namespace pddl {

class Action : public ParamCond {
 public:
  Condition *pre, *eff, *observe;

  Action(const std::string& s) : ParamCond(s), pre(0), eff(0), observe(0) {}

  Action(ParamCond* c) : ParamCond(c), pre(0), eff(0), observe(0) {}

  Action(const Action* a, Domain& d)
      : ParamCond(a), pre(0), eff(0), observe(0) {
    if (a->pre) {
      pre = a->pre->copy(d);
    }
    if (a->eff) {
      eff = a->eff->copy(d);
    }
    if (a->observe) {
      observe = a->observe->copy(d);
    }
  }

  virtual ~Action() {
    if (pre) {
      delete pre;
    }
    if (eff) {
      delete eff;
    }
    if (observe) {
      delete observe;
    }
  }

  void print(std::ostream& s) const {
    s << name << params << "\n";
    s << "Pre: " << pre;
    if (eff) {
      s << "Eff: " << eff;
    }
    if (observe) {
      s << "Obs: " << observe;
    }
  }

  virtual double duration() { return 1; }

  void PDDLPrint(std::ostream& s, unsigned indent,
                 const TokenStruct<std::string>& ts,
                 const Domain& d) const override;

  cocg_ast::Node::SharedPtr getTree(
      cocg_ast::Tree& tree, const Domain& d,
      const std::vector<std::string>& replace = {}) const override;

  void parseConditions(Stringreader& f, TokenStruct<std::string>& ts,
                       Domain& d);

  void parse(Stringreader& f, TokenStruct<std::string>& ts, Domain& d);

  void addParams(int m, unsigned n) {}

  void addParams(const IntVec& v) {
    if (pre) {
      pre->addParams(params.size(), v.size());
    }
    if (eff) {
      eff->addParams(params.size(), v.size());
    }
    if (observe) {
      observe->addParams(params.size(), v.size());
    }
    params.insert(params.end(), v.begin(), v.end());
  }

  Condition* copy(Domain& d) { return new Action(this, d); }

  CondVec precons();

  CondVec effects();

  GroundVec addEffects();

  GroundVec deleteEffects();

 protected:
  CondVec getSubconditionsFromCondition(Condition* c);

  GroundVec getGroundsFromCondition(Condition* c, bool neg);
};

typedef std::vector<Action*> ActionVec;

}  // namespace pddl
}  // namespace parser

#endif  // COCG_PDDL_PARSER_ACTION_H_
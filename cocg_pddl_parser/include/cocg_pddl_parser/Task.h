#ifndef COCG_PDDL_PARSER_TASK_H_
#define COCG_PDDL_PARSER_TASK_H_

#include "cocg_ast/node.h"
#include "cocg_ast/tree.h"
#include "cocg_pddl_parser/ParamCond.h"

namespace parser {
namespace pddl {

class Task : public ParamCond {
 public:
  Task() {}

  Task(const std::string& s) : ParamCond(s) {}

  Task(const ParamCond* c) : ParamCond(c) {}

  void PDDLPrint(std::ostream& s, unsigned indent,
                 const TokenStruct<std::string>& ts,
                 const Domain& d) const override {}

  cocg_ast::Node::SharedPtr getTree(
      cocg_ast::Tree& tree, const Domain& d,
      const std::vector<std::string>& replace = {}) const override {
    throw UnsupportedConstruct("Task");
  }

  void parse(Stringreader& f, TokenStruct<std::string>& ts, Domain& d) {}

  void addParams(int m, unsigned n) {}

  Condition* copy(Domain& d) { return new Task(this); }
};

typedef std::vector<Task*> TaskVec;

}  // namespace pddl
}  // namespace parser

#endif  // COCG_PDDL_PARSER_TASK_H_

#ifndef COCG_PDDL_PARSER_CONDITER_H_
#define COCG_PDDL_PARSER_CONDITER_H_

#include "cocg_pddl_parser/Condition.h"

namespace parser {
namespace pddl {

typedef std::list<std::pair<Condition *, unsigned>> CondList;

class CondIter : public std::iterator<std::input_iterator_tag, Condition *> {
 public:
  CondList condStack;

  CondIter(Condition *c) {
    condStack.push_back(std::make_pair(c, 0));
    (*this)++;
  }

  CondIter(const CondIter &ci) : condStack(ci.condStack) {}

  MyIterator &operator++() {
    while (condStack().size() && condStack.last().done()) {
      condStack.pop_back();
    }

    if (condStack().size()) {
    }

    return *this;
  }

  bool hasNext() { return condStack.size(); }

  Condition *operator*() { return condStack.last(); }
}

}  // namespace pddl
}  // namespace parser

#endif  // COCG_PDDL_PARSER_CONDITER_H_
#ifndef COCG_PDDL_PARSER_TYPEGROUND_H_
#define COCG_PDDL_PARSER_TYPEGROUND_H_

#include "cocg_ast/node.h"
#include "cocg_ast/tree.h"
#include "cocg_pddl_parser/Ground.h"

namespace parser {
namespace pddl {

class TypeGround : public Ground {
 public:
  TypeGround() : Ground() {}

  TypeGround(Lifted* l, const IntVec& p = IntVec()) : Ground(l, p) {}

  //	TypeGround( const TypeGround * tg )
  //		: Ground( tg ) {}

  void PDDLPrint(std::ostream& s, unsigned indent,
                 const TokenStruct<std::string>& ts,
                 const Domain& d) const override;

  cocg_ast::Node::SharedPtr getTree(
      cocg_ast::Tree& tree, const Domain& d,
      const std::vector<std::string>& replace = {}) const override;

  void insert(Domain& d, const StringVec& v);

  void parse(Stringreader& f, TokenStruct<std::string>& ts, Domain& d);
};

typedef std::vector<TypeGround*> TypeGroundVec;

}  // namespace pddl
}  // namespace parser

#endif  // COCG_PDDL_PARSER_TYPEGROUND_H_
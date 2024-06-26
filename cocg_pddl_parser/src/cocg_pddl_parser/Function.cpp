#include "cocg_pddl_parser/Domain.h"

namespace parser {
namespace pddl {

void Function::PDDLPrint(std::ostream& s, unsigned indent,
                         const TokenStruct<std::string>& ts,
                         const Domain& d) const {
  Lifted::PDDLPrint(s, indent, ts, d);
  if (returnType >= 0) {
    s << " - " << d.types[returnType]->name;
  }
}

cocg_ast::Node::SharedPtr Function::getTree(
    cocg_ast::Tree& tree, const Domain& d,
    const std::vector<std::string>& replace) const {
  throw UnsupportedConstruct("Function");
}

void Function::parse(Stringreader& f, TokenStruct<std::string>& ts, Domain& d) {
  Lifted::parse(f, ts, d);

  f.next();
  if (f.getChar() == '-') {
    f.assert_token("-");
    std::string s = f.getToken();
    if (s != "number") {
      f.c -= s.size();
      returnType = d.types.index(f.getToken(d.types));
    }
  }
}

}  // namespace pddl
}  // namespace parser

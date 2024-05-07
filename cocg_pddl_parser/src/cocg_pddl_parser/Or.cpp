#include "cocg_pddl_parser/Domain.h"

namespace parser {
namespace pddl {

void Or::PDDLPrint(std::ostream& s, unsigned indent,
                   const TokenStruct<std::string>& ts, const Domain& d) const {
  tabindent(s, indent);
  s << "( or\n";
  if (first) {
    first->PDDLPrint(s, indent + 1, ts, d);
  } else {
    tabindent(s, indent + 1);
    s << "()";
  }
  s << "\n";
  if (second) {
    second->PDDLPrint(s, indent + 1, ts, d);
  } else {
    tabindent(s, indent + 1);
    s << "()";
  }
  s << "\n";
  tabindent(s, indent);
  s << ")";
}

cocg_ast::Node::SharedPtr Or::getTree(
    cocg_ast::Tree& tree, const Domain& d,
    const std::vector<std::string>& replace) const {
  cocg_ast::Node::SharedPtr node = std::make_shared<cocg_ast::Node>();
  node->node_type = cocg_ast::Node::OR;
  node->node_id = tree.nodes.size();
  tree.nodes.push_back(*node);

  cocg_ast::Node::SharedPtr child_f = first->getTree(tree, d, replace);
  tree.nodes[node->node_id].children.push_back(child_f->node_id);
  cocg_ast::Node::SharedPtr child_s = second->getTree(tree, d, replace);
  tree.nodes[node->node_id].children.push_back(child_s->node_id);

  return node;
}

void Or::parse(Stringreader& f, TokenStruct<std::string>& ts, Domain& d) {
  f.next();
  f.assert_token("(");
  if (f.getChar() != ')') {
    first = d.createCondition(f);
    first->parse(f, ts, d);
  } else {
    ++f.c;
  }

  f.next();
  f.assert_token("(");
  if (f.getChar() != ')') {
    second = d.createCondition(f);
    second->parse(f, ts, d);
  } else {
    ++f.c;
  }

  f.next();
  f.assert_token(")");
}

}  // namespace pddl
}  // namespace parser

#include "cocg_pddl_parser/Domain.h"

namespace parser {
namespace pddl {

void Unknown::PDDLPrint(std::ostream& s, unsigned indent,
                        const TokenStruct<std::string>& ts,
                        const Domain& d) const {
  tabindent(s, indent);
  s << "( unknown ";
  conds[0]->PDDLPrint(s, 0, ts, d);
  tabindent(s, indent);
  s << ")";
}

cocg_ast::Node::SharedPtr Unknown::getTree(
    cocg_ast::Tree& tree, const Domain& d,
    const std::vector<std::string>& replace) const {
  cocg_ast::Node::SharedPtr node = std::make_shared<cocg_ast::Node>();
  node->node_id = tree.nodes.size();
  tree.nodes.push_back(*node);

  auto child = conds[0]->getTree(tree, d, replace);
  node->children.push_back(child->node_id);
  node->node_type = cocg_ast::Node::UNKNOWN;

  tree.nodes[node->node_id] = *node;
  return node;
}

void Unknown::parse(Stringreader& f, TokenStruct<std::string>& ts, Domain& d) {
  f.next();
  f.assert_token("(");
  Condition* condition = new TypeGround(d.preds.get(f.getToken(d.preds)));
  condition->parse(f, ts, d);
  conds.push_back(condition);

  f.assert_token(")");
}

}  // namespace pddl
}  // namespace parser

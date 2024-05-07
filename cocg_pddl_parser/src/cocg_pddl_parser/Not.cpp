#include "cocg_pddl_parser/Domain.h"

namespace parser {
namespace pddl {

void Not::PDDLPrint(std::ostream &s, unsigned indent,
                    const TokenStruct<std::string> &ts, const Domain &d) const {
  tabindent(s, indent);
  s << "( not ";
  if (cond) {
    cond->PDDLPrint(s, 0, ts, d);
  }
  s << " )";
}

cocg_ast::Node::SharedPtr Not::getTree(
    cocg_ast::Tree &tree, const Domain &d,
    const std::vector<std::string> &replace) const {
  cocg_ast::Node::SharedPtr node = std::make_shared<cocg_ast::Node>();
  node->node_type = cocg_ast::Node::NOT;
  node->node_id = tree.nodes.size();
  tree.nodes.push_back(*node);

  if (cond) {
    cocg_ast::Node::SharedPtr child = cond->getTree(tree, d, replace);
    tree.nodes[node->node_id].children.push_back(child->node_id);
  }

  return node;
}

void Not::parse(Stringreader &f, TokenStruct<std::string> &ts, Domain &d) {
  f.next();
  f.assert_token("(");

  cond = dynamic_cast<Ground *>(d.createCondition(f));

  if (!cond) {
    f.tokenExit(f.getToken());
  }

  cond->parse(f, ts, d);

  f.next();
  f.assert_token(")");
}

}  // namespace pddl
}  // namespace parser

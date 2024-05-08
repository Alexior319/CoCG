
#ifndef COCG_CORE_PROBLEM_EXPERT_H_
#define COCG_CORE_PROBLEM_EXPERT_H_

#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

#include "cocg_ast/node.h"
#include "cocg_ast/param.h"
#include "cocg_ast/tree.h"
#include "cocg_core/cocg_domain_expert.h"
#include "cocg_core/types.h"
#include "cocg_pddl_parser/Condition.h"
#include "cocg_pddl_parser/Utils.h"

namespace cocg {

class ProblemExpert {
 public:
  ProblemExpert(){};
  explicit ProblemExpert(std::shared_ptr<DomainExpert>& domain_expert);

  std::vector<cocg::Instance> getInstances();
  bool addInstance(const cocg::Instance& instance);
  bool removeInstance(const cocg::Instance& instance);
  std::optional<cocg::Instance> getInstance(const std::string& name);

  std::vector<cocg::Predicate> getPredicates();
  bool addPredicate(const cocg::Predicate& predicate);
  bool removePredicate(const cocg::Predicate& predicate);
  bool existPredicate(const cocg::Predicate& predicate);
  std::optional<cocg::Predicate> getPredicate(const std::string& expr);

  std::vector<cocg_ast::Tree> getConditionals();
  bool addConditional(const cocg_ast::Tree& condition);
  bool removeConditional(const cocg_ast::Tree& condition);
  bool existConditional(const cocg_ast::Tree& condition);

  std::vector<cocg::Function> getFunctions();
  bool addFunction(const cocg::Function& function);
  bool removeFunction(const cocg::Function& function);
  bool existFunction(const cocg::Function& function);
  bool updateFunction(const cocg::Function& function);
  std::optional<cocg::Function> getFunction(const std::string& expr);

  cocg::Goal getGoal();
  bool setGoal(const cocg::Goal& goal);
  bool isGoalSatisfied(const cocg::Goal& goal);

  bool clearGoal();
  bool clearKnowledge();

  std::string getProblem();
  bool addProblem(const std::string& problem_str);

  bool existInstance(const std::string& name);
  bool isValidType(const std::string& type);
  bool isValidPredicate(const cocg::Predicate& predicate);
  bool isValidFunction(const cocg::Function& function);
  bool isValidGoal(const cocg::Goal& goal);
  bool isValidCondition(const cocg_ast::Tree& cond);

 private:
  bool checkPredicateTreeTypes(const cocg_ast::Tree& tree,
                               std::shared_ptr<DomainExpert>& domain_expert_,
                               uint8_t node_id = 0);

  void removeInvalidPredicates(std::vector<cocg::Predicate>& predicates,
                               const cocg::Instance& instance);
  void removeInvalidFunctions(std::vector<cocg::Function>& functions,
                              const cocg::Instance& instance);
  void removeInvalidGoals(const cocg::Instance& instance);

  std::vector<cocg::Instance> instances_;
  std::vector<cocg::Predicate> predicates_;
  std::vector<cocg::Function> functions_;
  std::vector<cocg_ast::Tree> conditionals_;
  cocg::Goal goal_;

  std::shared_ptr<DomainExpert> domain_expert_;
};

}  // namespace cocg

#endif  // COCG_CORE_PROBLEM_EXPERT_H_

#ifndef COCG_CORE_DOMAIN_EXPERT_H_
#define COCG_CORE_DOMAIN_EXPERT_H_

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "cocg_ast/action.h"
#include "cocg_ast/durative_action.h"
#include "cocg_ast/node.h"
#include "cocg_domain_reader.h"
#include "cocg_pddl_parser/Domain.h"
#include "cocg_core/types.h"

namespace cocg {

/// DomainExpert is in charge of managing the internal structure of a domain.
class DomainExpert {
 public:
  /// Create a new DomainExpert with the content of a domain.
  /**
   * \param[in] node_name The content of a PDDL domain.
   */
  explicit DomainExpert(const std::string& domain);

  /// Extend the content of aDomainExpert with the content of another domain.
  /**
   * \param[in] node_name The content of a PDDL domain.
   */
  void extendDomain(const std::string& domain);

  /// Get the domain name.
  /**
   * \return A string containing the domain name.
   */
  std::string getName();

  /// Get the types existing in the domain.
  /**
   * \return The vector containing the names of the types.
   */
  std::vector<std::string> getTypes();

  /// Get the details of a constants existing for a type.
  /**
   * \param[in] predicate The name of the type.
   * \return A list of constants names for the passed type
   */
  std::vector<std::string> getConstants(const std::string& type);

  /// Get the predicates existing in the domain.
  /**
   * \return The vector containing the name of the predicates.
   */
  std::vector<cocg::Predicate> getPredicates();

  /// Get the details of a predicate existing in the domain.
  /**
   * \param[in] predicate The name of the predicate.
   * \return A Predicate object containing the predicate name and its parameters
   * (name and type). If the predicate does not exist, the value returned has
   * not value.
   */
  std::optional<cocg::Predicate> getPredicate(const std::string& predicate);

  /// Get the functions existing in the domain.
  /**
   * \return The vector containing the name of the functions.
   */
  std::vector<cocg::Function> getFunctions();

  /// Get the details of a function existing in the domain.
  /**
   * \param[in] function The name of the function.
   * \return A Function object containing the function name and its parameters
   * (name and type). If the function does not exist, the value returned has not
   * value.
   */
  std::optional<cocg::Function> getFunction(const std::string& function);

  /// Get the regular actions existing in the domain.
  /**
   * \return The vector containing the names of the actions.
   */
  std::vector<std::string> getActions();

  /// Get the details of an regular action existing in the domain.
  /**
   * \param[in] action The name of the action.
   * \return An Action object containing the action name, parameters,
   * requirements and effects. If the action does not exist, the value returned
   * has not value.
   */
  cocg_ast::Action::SharedPtr getAction(
      const std::string& action, const std::vector<std::string>& params = {}) const;

  /// Get the temporal actions existing in the domain.
  /**
   * \return The vector containing the names of the actions.
   */
  std::vector<std::string> getDurativeActions();

  /// Get the details of an durative action existing in the domain.
  /**
   * \param[in] action The name of the action.
   * \return A Durative Action object containing the action name, parameters,
   * requirements and effects. If the action does not exist, the value returned
   * has not value.
   */
  cocg_ast::DurativeAction::SharedPtr getDurativeAction(
      const std::string& action, const std::vector<std::string>& params = {});

  /// Get the current domain, ready to be saved to file, or to initialize
  /// another domain.
  /**
   * \return A string containing the domain.
   */
  std::string getDomain();

  /// Determine if a particular domain exists.
  /**
   * \param[in] domain The name of the domain.
   * \return true if the domain exists.
   */
  bool existDomain(const std::string& domain_name);

 private:
  std::shared_ptr<parser::pddl::Domain> domain_;
  DomainReader domains_;
};

}  // namespace cocg

#endif  // COCG_CORE_DOMAIN_EXPERT_H_


#include "cocg_core/cocg_domain_expert.h"

#include <algorithm>
#include <memory>
#include <optional>
#include <string>
#include <typeinfo>
#include <vector>

namespace cocg {

DomainExpert::DomainExpert(const std::string &domain) { extendDomain(domain); }

void DomainExpert::extendDomain(const std::string &domain) {
  domains_.add_domain(domain);

  domain_ = std::make_shared<parser::pddl::Domain>();

  try {
    domain_->parse(domains_.get_joint_domain());
  } catch (const std::exception &e) {
    std::cerr << "\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\nError parsing PDDL: "
              << e.what() << std::endl;
    std::cerr << "Error parsing PDDL: " << e.what() << std::endl;
  }
}

std::string DomainExpert::getName() { return domain_->name; }

std::vector<std::string> DomainExpert::getTypes() {
  std::vector<std::string> ret;
  if (domain_->typed) {
    for (unsigned i = 1; i < domain_->types.size(); i++) {
      ret.push_back(domain_->types[i]->name);
    }
  }
  return ret;
}

std::vector<std::string> DomainExpert::getConstants(const std::string &type) {
  if (!domain_->typed) {
    return std::vector<std::string>();
  }

  return domain_->getType(type)->constants.tokens;
}

std::vector<cocg::Predicate> DomainExpert::getPredicates() {
  std::vector<cocg::Predicate> ret;
  for (unsigned i = 0; i < domain_->preds.size(); i++) {
    cocg_ast::Node pred;
    pred.node_type = cocg_ast::Node::PREDICATE;
    pred.name = domain_->preds[i]->name;
    ret.push_back(pred);
  }
  return ret;
}

std::optional<cocg::Predicate> DomainExpert::getPredicate(
    const std::string &predicate) {
  std::string predicate_search = predicate;
  std::transform(predicate_search.begin(), predicate_search.end(),
                 predicate_search.begin(), ::tolower);

  cocg_ast::Node ret;
  bool found = false;
  unsigned i = 0;

  while (i < domain_->preds.size() && !found) {
    if (domain_->preds[i]->name == predicate_search) {
      found = true;
      ret.name = predicate_search;
      for (unsigned j = 0; j < domain_->preds[i]->params.size(); j++) {
        cocg_ast::Param param;
        param.name = "?" +
                     domain_->types[domain_->preds[i]->params[j]]->getName() +
                     std::to_string(j);
        param.type = domain_->types[domain_->preds[i]->params[j]]->name;
        domain_->types[domain_->preds[i]->params[j]]->getSubTypesNames(
            param.sub_types);
        ret.parameters.push_back(param);
      }
    }
    i++;
  }

  if (found) {
    return ret;
  } else {
    return {};
  }
}

std::vector<cocg::Function> DomainExpert::getFunctions() {
  std::vector<cocg::Function> ret;
  for (unsigned i = 0; i < domain_->funcs.size(); i++) {
    cocg_ast::Node func;
    func.node_type = cocg_ast::Node::FUNCTION;
    func.name = domain_->funcs[i]->name;
    ret.push_back(func);
  }
  return ret;
}

std::optional<cocg::Function> DomainExpert::getFunction(
    const std::string &function) {
  std::string function_search = function;
  std::transform(function_search.begin(), function_search.end(),
                 function_search.begin(), ::tolower);

  cocg::Function ret;
  bool found = false;
  unsigned i = 0;

  while (i < domain_->funcs.size() && !found) {
    if (domain_->funcs[i]->name == function_search) {
      found = true;
      ret.name = function_search;
      for (unsigned j = 0; j < domain_->funcs[i]->params.size(); j++) {
        cocg_ast::Param param;
        param.name = "?" +
                     domain_->types[domain_->funcs[i]->params[j]]->getName() +
                     std::to_string(j);
        param.type = domain_->types[domain_->funcs[i]->params[j]]->name;
        domain_->types[domain_->funcs[i]->params[j]]->getSubTypesNames(
            param.sub_types);
        ret.parameters.push_back(param);
      }
    }
    i++;
  }

  if (found) {
    return ret;
  } else {
    return {};
  }
}

std::vector<std::string> DomainExpert::getActions() {
  std::vector<std::string> ret;
  for (unsigned i = 0; i < domain_->actions.size(); i++) {
    bool is_action = dynamic_cast<parser::pddl::TemporalAction *>(
                         domain_->actions[i]) == nullptr;
    parser::pddl::Action *action_obj =
        dynamic_cast<parser::pddl::Action *>(domain_->actions[i]);
    if (is_action) {
      ret.push_back(domain_->actions[i]->name);
    }
  }
  return ret;
}

cocg_ast::Action::SharedPtr DomainExpert::getAction(
    const std::string &action, const std::vector<std::string> &params) const{
  std::string action_search = action;
  std::transform(action_search.begin(), action_search.end(),
                 action_search.begin(), ::tolower);

  auto ret = std::make_shared<cocg_ast::Action>();
  bool found = false;
  unsigned i = 0;

  while (i < domain_->actions.size() && !found) {
    bool is_action = dynamic_cast<parser::pddl::TemporalAction *>(
                         domain_->actions[i]) == nullptr;
    parser::pddl::Action *action_obj =
        dynamic_cast<parser::pddl::Action *>(domain_->actions[i]);

    if (is_action && action_obj->name == action_search) {
      found = true;
      ret->name = action;

      // Parameters
      for (unsigned j = 0; j < action_obj->params.size(); j++) {
        cocg_ast::Param param;
        if (j < params.size()) {
          param.name = params[j];
        } else {
          param.name = "?" + std::to_string(j);
        }
        param.type = domain_->types[action_obj->params[j]]->name;
        domain_->types[action_obj->params[j]]->getSubTypesNames(
            param.sub_types);
        ret->parameters.push_back(param);
      }

      // Preconditions
      if (action_obj->pre) {
        action_obj->pre->getTree(ret->preconditions, *domain_, params);
      }

      // Effects
      if (action_obj->eff) {
        action_obj->eff->getTree(ret->effects, *domain_, params);
      }

      // Observe
      if (action_obj->observe) {
        action_obj->observe->getTree(ret->observe, *domain_, params);
      }
    }
    i++;
  }

  if (found) {
    return ret;
  } else {
    return {};
  }
}

std::vector<std::string> DomainExpert::getDurativeActions() {
  std::vector<std::string> ret;
  for (unsigned i = 0; i < domain_->actions.size(); i++) {
    bool is_durative_action = dynamic_cast<parser::pddl::TemporalAction *>(
                                  domain_->actions[i]) != nullptr;
    parser::pddl::TemporalAction *action_obj =
        dynamic_cast<parser::pddl::TemporalAction *>(domain_->actions[i]);
    if (is_durative_action) {
      ret.push_back(domain_->actions[i]->name);
    }
  }
  return ret;
}

cocg_ast::DurativeAction::SharedPtr DomainExpert::getDurativeAction(
    const std::string &action, const std::vector<std::string> &params) {
  std::string action_search = action;
  std::transform(action_search.begin(), action_search.end(),
                 action_search.begin(), ::tolower);

  auto ret = std::make_shared<cocg_ast::DurativeAction>();
  bool found = false;
  unsigned i = 0;

  while (i < domain_->actions.size() && !found) {
    bool is_durative_action = dynamic_cast<parser::pddl::TemporalAction *>(
                                  domain_->actions[i]) != nullptr;
    parser::pddl::TemporalAction *action_obj =
        dynamic_cast<parser::pddl::TemporalAction *>(domain_->actions[i]);

    if (is_durative_action && action_obj->name == action_search) {
      found = true;
      ret->name = action;

      // Parameters
      for (unsigned j = 0; j < action_obj->params.size(); j++) {
        cocg_ast::Param param;
        if (j < params.size()) {
          param.name = params[j];
        } else {
          param.name = "?" + std::to_string(j);
        }
        param.type = domain_->types[action_obj->params[j]]->name;
        domain_->types[action_obj->params[j]]->getSubTypesNames(
            param.sub_types);
        ret->parameters.push_back(param);
      }

      // Preconditions AtStart
      if (action_obj->pre) {
        action_obj->pre->getTree(ret->at_start_requirements, *domain_, params);
      }

      // Preconditions OverAll
      if (action_obj->pre_o) {
        action_obj->pre_o->getTree(ret->over_all_requirements, *domain_,
                                   params);
      }

      // Preconditions AtEnd
      if (action_obj->pre_e) {
        action_obj->pre_e->getTree(ret->at_end_requirements, *domain_, params);
      }

      // Effects AtStart
      if (action_obj->eff) {
        action_obj->eff->getTree(ret->at_start_effects, *domain_, params);
      }

      // Effects AtEnd
      if (action_obj->eff_e) {
        action_obj->eff_e->getTree(ret->at_end_effects, *domain_, params);
      }

      // Observe
      if (action_obj->observe) {
        action_obj->observe->getTree(ret->observe, *domain_, params);
      }
    }
    i++;
  }

  if (found) {
    return ret;
  } else {
    return {};
  }
}

std::string DomainExpert::getDomain() { return domains_.get_joint_domain(); }

bool DomainExpert::existDomain(const std::string &domain_name) {
  for (auto domain : domains_.get_domains()) {
    if (domain_name == domain.name) {
      return true;
    }
  }
  return false;
}

}  // namespace cocg


#ifndef COCG_CORE_DOMAIN_READER_H_
#define COCG_CORE_DOMAIN_READER_H_

#include <string>
#include <vector>

namespace cocg {

struct Domain {
  std::string name;
  std::string requirements;
  std::string types;
  std::string constants;
  std::string predicates;
  std::string functions;
  std::vector<std::string> actions;
};

class DomainReader {
 public:
  DomainReader();

  void add_domain(const std::string& domain);
  std::string get_joint_domain() const;
  std::vector<Domain> get_domains() { return domains_; }

 protected:
  int get_end_block(const std::string& domain, std::size_t init_pos);

  std::string get_name(std::string& domain);
  std::string get_requirements(std::string& domain);
  std::string get_types(const std::string& domain);
  std::string get_constants(const std::string& domain);
  std::string get_predicates(const std::string& domain);
  std::string get_functions(const std::string& domain);
  std::vector<std::string> get_actions(const std::string& domain);

 private:
  std::vector<Domain> domains_;
};

}  // namespace cocg

#endif  // COCG_CORE_DOMAIN_READER_H_

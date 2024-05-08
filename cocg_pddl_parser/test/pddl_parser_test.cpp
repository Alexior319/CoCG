
#include <algorithm>
#include <filesystem>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "cocg_pddl_parser/Instance.h"
#include "gtest/gtest.h"

class PDDLParserTestCase : public ::testing::Test {
 protected:
  static void SetUpTestCase() {}
};

TEST(PDDLParserTestCase, pddl_parser) {
  std::string pkgpath = std::filesystem::current_path().string();
  std::string domain_file = pkgpath + "/pddl_parser/dom1.pddl";
  std::string instance_file = pkgpath + "/pddl_parser/prob1.pddl";

  std::ifstream domain_ifs(domain_file);
  ASSERT_TRUE(domain_ifs.good());
  std::string domain_str((std::istreambuf_iterator<char>(domain_ifs)),
                         std::istreambuf_iterator<char>());
  ASSERT_NE(domain_str, "");
  std::ifstream instance_ifs(instance_file);
  ASSERT_TRUE(instance_ifs.good());
  std::string instance_str((std::istreambuf_iterator<char>(instance_ifs)),
                           std::istreambuf_iterator<char>());

  ASSERT_NE(instance_str, "");
  // Read domain and instance
  bool okparse = false;
  bool okprint = false;
  try {
    parser::pddl::Domain domain(domain_str);
    parser::pddl::Instance instance(domain, instance_str);
    okparse = true;
    try {
      std::cout << domain << std::endl;
      std::cout << instance << std::endl;
      okprint = true;
    } catch (std::runtime_error e) {
    }
  } catch (std::runtime_error e) {
  }
  ASSERT_TRUE(okparse);
  ASSERT_TRUE(okprint);
}

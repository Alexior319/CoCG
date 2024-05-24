#include "cff_cocg.h"

void test_cff_cocg_method() {
  std::string pkgpath = std::filesystem::current_path().string();
  std::string domain_path = pkgpath + "/pddl/cocg/d_blocks.pddl";

  std::string problem_path = pkgpath + "/pddl/cocg/p_blocks_r2p3b2.pddl";

  cff_cocg(domain_path, problem_path);
}

int main() {
  test_cff_cocg_method();
  return 0;
}
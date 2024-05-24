#include "cff_cocg.h"

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cout << "Usage: cff_cocg [domain_path] [problem_path]" << std::endl;
    return -1;
  }

  std::string domain_path(argv[1]);
  std::string problem_path(argv[2]);
  cff_cocg(domain_path, problem_path);

  return 0;
}
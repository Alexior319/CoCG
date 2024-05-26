#include "cff_cocg.h"

void cff_cocg(const std::string& domain_path, const std::string& problem_path) {
  std::ifstream domain_ifs(domain_path);
  std::string domain_str((std::istreambuf_iterator<char>(domain_ifs)),
                         std::istreambuf_iterator<char>());

  std::ifstream problem_ifs(problem_path);
  std::string problem_str((std::istreambuf_iterator<char>(problem_ifs)),
                          std::istreambuf_iterator<char>());

  auto domain_expert = std::make_shared<cocg::DomainExpert>(domain_str);
  auto problem_expert_init =
      std::make_shared<cocg::ProblemExpert>(domain_expert);
  problem_expert_init->addProblem(problem_str);

  // start planning using contingent ff
  std::cout << "[CFF] Initialized!, start planning using Contingent-FF. "
            << std::endl;
  auto start = std::chrono::high_resolution_clock::now();

  auto cff_plan_sovler = std::make_shared<cocg::CFFPlanSolver>();
  auto cff_plan_tree_root =
      cff_plan_sovler->get_cocg_cont_plan_tree_root(domain_str, problem_str);

  if (cff_plan_tree_root == nullptr) {
    std::cout << "[CFF] Problem unsolvable by Contingent-FF, exiting..."
              << std::endl;
    return;
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::ratio<1, 1>> cff_duration(end - start);
  auto cff_planning_duration_s = cff_duration.count();
  std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
  std::cout << "CFF has finished planning in the first stage, planning time: "
            << cff_planning_duration_s << " (seconds)" << std::endl;
  std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
  std::cout << "[CFF->CoCG] CFF planning done, start optimizing using CoCG..."
            << std::endl;

  start = std::chrono::high_resolution_clock::now();
  uint32_t t0 = 0;

  auto ret = cocg::traverse_contingent_planning_tree(
      problem_expert_init, cff_plan_tree_root, *domain_expert);
  auto root_goal_state = std::get<0>(ret);
  auto root_actions_set = std::get<1>(ret);
  auto root_next_node = std::get<2>(ret);

  auto cocg_subgraph_root = cocg::build_cocg_subgraph(
      problem_expert_init, root_goal_state, root_actions_set, root_next_node,
      t0, *domain_expert);
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::ratio<1, 1>> cocg_duration(end - start);
  auto cocg_duration_s = cocg_duration.count();

  std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
  std::cout << "[CFF] Problem solved by Contingent-FF, tree tructure:"
            << std::endl;
  cocg::print_cont_plan_tree(cff_plan_tree_root);
  std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";

  std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
  std::cout << "[CoCG] The transformed graph is as follows:" << std::endl;
  cocg::print_cocg_graph(cocg_subgraph_root);
  std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";

  std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
  std::cout << "First stage, CFF planning time: " << cff_planning_duration_s
            << " (seconds)" << std::endl;
  std::cout << "Second stage, graph-based planning time: " << cocg_duration_s
            << " (seconds)" << std::endl;
  std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
}

#include "cocg_core/cocg_core.h"

namespace cocg {
std::shared_ptr<SubGraphNode> build_cocg_subgraph(
    std::shared_ptr<cocg::ProblemExpert> mid_expert,
    std::vector<cocg_ast::Action::SharedPtr> actions,
    cocg::ContPlanNode::SharedPtr node, float t0,
    cocg::DomainExpert& domain_expert) {
  std::shared_ptr<SubGraphNode> ret = std::make_shared<SubGraphNode>();
  ret->t0_ = t0;

  std::vector<std::vector<cocg_ast::Action>> action_layers =
      compute_planning_graph(mid_expert, actions, domain_expert);

  int i = 1, k = action_layers.size();
  for (; i <= k; i++) {
    ret->actions_layers_[i] = action_layers[i];
  }

  ret->layers_cnt_ = k + 1;
  if (node != nullptr) {
    cocg_ast::Action::SharedPtr action =
        convert_plan_node_to_ast(node, domain_expert);
    ret->actions_layers_[k + 1] = {*action};

    // traverse the true part
    std::shared_ptr<cocg::ProblemExpert> next_true_mid_expert =
        apply_sensing_action(mid_expert, *action, true, true);
    std::tuple<std::shared_ptr<cocg::ProblemExpert>,
               std::vector<cocg_ast::Action::SharedPtr>,
               cocg::ContPlanNode::SharedPtr>
        next_true_tuple = traverse_contingent_planning_tree(
            next_true_mid_expert, node->true_node, domain_expert);
    ret->next_true_ = build_cocg_subgraph(
        next_true_mid_expert, std::get<1>(next_true_tuple),
        std::get<2>(next_true_tuple), t0 + ret->layers_cnt_, domain_expert);

    // traverse the false part
    std::shared_ptr<cocg::ProblemExpert> next_false_mid_expert =
        apply_sensing_action(mid_expert, *action, false, true);
    std::tuple<std::shared_ptr<cocg::ProblemExpert>,
               std::vector<cocg_ast::Action::SharedPtr>,
               cocg::ContPlanNode::SharedPtr>
        next_false_tuple = traverse_contingent_planning_tree(
            next_false_mid_expert, node->false_node, domain_expert);
    ret->next_true_ = build_cocg_subgraph(
        next_false_mid_expert, std::get<1>(next_true_tuple),
        std::get<2>(next_true_tuple), t0 + ret->layers_cnt_, domain_expert);
  } else {
    ret->actions_layers_[k + 1] = {};
  }
  return ret;
}

std::vector<std::vector<cocg_ast::Action>> compute_planning_graph(
    std::shared_ptr<cocg::ProblemExpert> mid_expert,
    const std::vector<cocg_ast::Action::SharedPtr> actions_p,
    cocg::DomainExpert& domain_expert) {
  // copy the actions pointers to a new vector
  std::vector<cocg_ast::Action> actions;
  for (auto action_p : actions_p) {
    actions.push_back(*action_p);
  }
  std::vector<std::vector<cocg_ast::Action>> action_layers;

  // TODO: compute parallelized planning graph
  // op_list ops;
  // int i, max_time = 0, auto_stop = 0, old_num_created, givedef = 0;
  // FILE* fp;
  // char opfile[50], factfile[50], option[10];

  // option[0] = opfile[0] = factfile[0] = '\0';
  // DEBUG_FLAG = -1;

  // ops = load_ops(fp);

  // if ((fp = fopen(factfile, "r")) == NULL) do_error("cant load facts file");
  // the_types = load_types(fp); /* load in types */

  // load_fact_list(fp, &initial_facts);         /* load in actual facts */
  // read_item(fp, junk);                        /* left paren */
  // read_item(fp, junk);                        /* "effects" */
  // num_goals = load_fact_list(fp, &the_goals); /* load in the goals */
  // if (num_goals > MAXGOALS) do_error("MAXGOALS too small");
  // printf("facts loaded.\n");

  // if (givedef && max_time == 0)
  //   auto_stop = 1; /* NUMBER OF TIME STEPS */
  // else if (max_time == 0) {
  //   printf("number of time steps, or <CR> for automatic: ");
  //   if (sscanf(junk, "%d", &max_time) != 1) {
  //     auto_stop = 1;
  //     max_time = 0;
  //   }
  // }

  // do_memo = 1; /* ALWAYS DOING MEMOIZING */

  // if (do_completeness == 0) printf("turning off completeness check.\n");
  // /* BEGIN TIMING */
  // auto start = std::chrono::high_resolution_clock::now();

  // /* DO PRE-PREPROCESSING IF DESIRED */
  // if (do_irrel) initial_facts = useful_facts(ops, initial_facts);

  // /* MAKE GRAPH AND START PLANNING */
  // max_time = create_graph(ops, initial_facts, max_time); /* treat 0 as auto*/
  // if (max_time == -1) {
  //   if (same_as_prev_flag)
  //     printf("Problem not solvable: can't even reach non-mutex goals.\n");
  //   else
  //     printf("Can't get there from here in allotted time.\n");
  // } else if (!auto_stop) {
  //   if (DEBUG_FLAG > 0) print_info(max_time);
  //   if (do_plan(max_time))
  //     print_plan(max_time);
  //   else
  //     print_cant_do(max_time);
  // } else {
  //   for (; max_time <= max_auto; max_time++) {
  //     if (DEBUG_FLAG > 0) print_info(max_time);
  //     /* try doing plan */
  //     if (do_plan(max_time)) {
  //       print_plan(max_time);
  //       break;
  //     } else {
  //       if (max_time == max_auto)
  //         print_cant_do(max_time);
  //       else
  //         printf("Can't solve in %d steps\n", max_time);
  //       if (same_as_prev_flag) { /* check for true unsolvability */
  //         if (num_hashes[0] == num_hashes[1] && do_completeness) {
  //           printf("Problem not solvable.\n");
  //           break;
  //         } else {
  //           num_hashes[0] = num_hashes[1];
  //         }
  //       }
  //     }
  //     /* create next level (unless at very end) */
  //     if (max_time != max_auto) {
  //       old_num_created = num_created;
  //       create_graph_layer(ops);
  //       printf("%d new nodes added.\n", num_created - old_num_created);
  //     }
  //   }
  // }

  // // END TIMING
  // auto end = std::chrono::high_resolution_clock::now();
  // auto duration =
  //     std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  // double duration_secs = (duration.count()) *
  //                        std::chrono::microseconds::period::num /
  //                        std::chrono::microseconds::period::den;

  // std::cout << "Time used in this graphplan optimization (seconds): "
  //           << duration_secs << std::endl;

  return action_layers;
}
}  // namespace cocg

#ifndef COCG_CORE_SUBGRAPH_H_
#define COCG_CORE_SUBGRAPH_H_

#include <map>
#include <memory>
#include <vector>

#include "cocg_core/cocg_problem_expert.h"
#include "cocg_ast/action.h"

// TODO
namespace cocg {
class SubGraphNode {
    public:

    private:
        float t0_;
        int layers_cnt_;
        std::shared_ptr<cocg::ProblemExpert> problem_expert_;
        std::map<float, std::vector<cocg_ast::Action>> actions_layers_;
        std::shared_ptr<SubGraphNode> next_true_;
        std::shared_ptr<SubGraphNode> next_false_;
};
}  // namespace cocg

#endif  // COCG_CORE_SUBGRAPH_H_

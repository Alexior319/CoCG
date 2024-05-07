
#include "cocg_core/utils.h"

#include <sstream>
#include <string>

namespace cocg {

std::vector<std::string> tokenize(const std::string& string,
                                  const std::string& delim) {
  std::string::size_type lastPos = 0,
                         pos = string.find_first_of(delim, lastPos);
  std::vector<std::string> tokens;

  while (lastPos != std::string::npos) {
    if (pos != lastPos) {
      tokens.push_back(string.substr(lastPos, pos - lastPos));
    }
    lastPos = pos;
    if (lastPos == std::string::npos || lastPos + 1 == string.length()) {
      break;
    }
    pos = string.find_first_of(delim, ++lastPos);
  }

  return tokens;
}

std::string substr_without_empty_lines(std::string string, std::size_t init_pos,
                                       std::size_t end_pos) {
  std::stringstream stream_in(string.substr(init_pos, end_pos - init_pos));
  std::stringstream stream_out;
  std::string line;
  bool first = true;
  while (std::getline(stream_in, line)) {
    if (!(line.empty() || line.find_first_not_of(' ') == std::string::npos)) {
      if (first) {
        first = false;
      } else {
        stream_out << "\n";
      }
      stream_out << line;
    }
  }
  return stream_out.str();
}

std::string remove_comments(const std::string& pddl) {
  std::stringstream uncomment;

  std::size_t pddl_length = pddl.length();
  std::size_t start_pos = 0;
  std::size_t end_pos = 0;
  bool commented = false;
  while (end_pos < pddl_length) {
    if ((!commented) && (pddl.at(end_pos) == ';')) {
      commented = true;
      uncomment << pddl.substr(start_pos, end_pos - start_pos);
    }

    if (commented && (pddl.at(end_pos) == '\r' || pddl.at(end_pos) == '\n')) {
      commented = false;
      start_pos = end_pos;
    }
    end_pos++;
  }
  if (!commented) {
    uncomment << pddl.substr(start_pos, end_pos - start_pos);
  }
  return std::string(uncomment.str());
}
cocg_ast::Plan encode_plan(const std::shared_ptr<ContPlanNode>& root) {
  cocg_ast::Plan plan;
  internal::encode_plan(root, plan.structure, plan.items);
  return plan;
}

std::shared_ptr<ContPlanNode> decode_plan(const cocg_ast::Plan& plan) {
  cocg_ast::Plan plan_copy = plan;
  return internal::decode_plan(plan_copy.structure, plan_copy.items);
}

namespace internal {
void encode_plan(const std::shared_ptr<ContPlanNode>& root,
                 std::vector<int>& struc,
                 std::vector<cocg_ast::PlanItem>& data) {
  if (root == nullptr) {
    struc.push_back(0);
    return;
  }
  struc.push_back(1);
  data.push_back(root->item);
  encode_plan(root->true_node, struc, data);
  if (root->true_node != root->false_node) {
    encode_plan(root->false_node, struc, data);
  } else {
    encode_plan(nullptr, struc, data);
  }
}

std::shared_ptr<ContPlanNode> decode_plan(
    std::vector<int>& struc, std::vector<cocg_ast::PlanItem>& data) {
  std::queue<int> struc_queue;
  std::queue<cocg_ast::PlanItem> data_queue;
  for (const auto& e : data) {
    data_queue.push(e);
  }

  for (const auto& e : struc) {
    struc_queue.push(e);
  }

  return decode_plan(struc_queue, data_queue);
}

std::shared_ptr<ContPlanNode> decode_plan(
    std::queue<int>& struc, std::queue<cocg_ast::PlanItem>& data) {
  if (struc.empty()) {
    return nullptr;
  }
  bool b = struc.front();
  struc.pop();
  if (b == 1) {
    cocg_ast::PlanItem key = data.front();
    data.pop();
    auto root = std::make_shared<ContPlanNode>();
    root->item = key;
    root->true_node = decode_plan(struc, data);
    root->false_node = decode_plan(struc, data);
    return root;
  }
  return nullptr;
}
}  // namespace internal

}  // namespace cocg

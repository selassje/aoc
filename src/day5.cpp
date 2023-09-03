#include "day5.hpp"

namespace aoc22::day5 {

std::pair<std::string, std::string>
solve(const Input& input)
{
  Stacks stacks_part1{ input.stacks };
  Stacks stacks_part2{ input.stacks };

  auto perform_move = [&stacks_part1](const Move& m) {
    for (auto i = 0UL; i < m.Quantity; ++i) {
      auto crate = stacks_part1[m.From - 1].top();
      stacks_part1[m.From - 1].pop();
      stacks_part1[m.To - 1].push(crate);
    }
  };

  auto perform_move_order_kept = [&stacks_part2](const Move& m) {
    std::vector<unsigned char> to_be_moved{};
    for (auto i = 0UL; i < m.Quantity; ++i) {
      auto crate = stacks_part2[m.From - 1].top();
      stacks_part2[m.From - 1].pop();
      to_be_moved.push_back(crate);
    }

    std::reverse(to_be_moved.begin(), to_be_moved.end());
    for (const auto& crate : to_be_moved) {
      stacks_part2[m.To - 1].push(crate);
    }
  };

  for (const auto& m : input.moves) {
    perform_move(m);
    perform_move_order_kept(m);
  }

  std::string result_part1{};
  std::string result_part2{};
  for (const auto& stack : stacks_part1) {
    result_part1 += stack.top();
  }
  for (const auto& stack : stacks_part2) {
    result_part2 += stack.top();
  }
  return std::make_pair(result_part1, result_part2);
}

};

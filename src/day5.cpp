#include "day5.hpp"

namespace aoc22::day5 {

std::string
solve(const Input& input)
{
  Stacks stacks{ input.stacks };

  auto perform_move = [&stacks](const Move& m) {
    for (auto i = 0UL; i < m.Quantity; ++i) {
      auto crate = stacks[m.From - 1].top();
      stacks[m.From - 1].pop();
      stacks[m.To - 1].push(crate);
    }
  };

  for (const auto& m : input.moves) {
    perform_move(m);
  }

  std::string result{};
  for (const auto& stack : stacks) {
    result += stack.top();
  }
  return result;
}

};

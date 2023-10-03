#include "day5.hpp"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

namespace aoc22::day5 {

std::pair<std::string, std::string>
solve(const Input& input)
{
  Stacks stacksPart1{ input.stacks };
  Stacks stacksPart2{ input.stacks };

  auto performMove = [&stacksPart1](const Move& m) {
    for (auto i = 0UL; i < m.quantity; ++i) {
      auto crate = stacksPart1[m.from - 1].top();
      stacksPart1[m.from - 1].pop();
      stacksPart1[m.to - 1].push(crate);
    }
  };

  auto performMoveOrderKept = [&stacksPart2](const Move& m) {
    std::vector<char> toBeMoved{};
    for (auto i = 0UL; i < m.quantity; ++i) {
      auto crate = stacksPart2[m.from - 1].top();
      stacksPart2[m.from - 1].pop();
      toBeMoved.push_back(crate);
    }

    std::reverse(toBeMoved.begin(), toBeMoved.end());
    for (const auto& crate : toBeMoved) {
      stacksPart2[m.to - 1].push(crate);
    }
  };

  for (const auto& m : input.moves) {
    performMove(m);
    performMoveOrderKept(m);
  }

  std::string resultPart1{};
  std::string resultPart2{};
  for (const auto& stack : stacksPart1) {
    resultPart1 += stack.top();
  }
  for (const auto& stack : stacksPart2) {
    resultPart2 += stack.top();
  }
  return std::make_pair(resultPart1, resultPart2);
}

};

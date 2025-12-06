#include "aoc25/day6.hpp"
#include <cstdint>

namespace {
  std::uint64_t performOperation(const aoc25::day6::Problem& problem)
  {
    std::uint64_t result = (problem.operation == aoc25::day6::Operation::Add) ? 0 : 1;
    for (const auto& operand : problem.operands) {
      if (problem.operation == aoc25::day6::Operation::Add) {
        result += operand;
      } else {
        result *= operand;
      }
    }
    return result;
  }
}


namespace aoc25::day6 {
Result
solve(const Input& input)
{
  std::uint64_t part1 = 0;
  for (const auto& problem : input) {
    part1 += performOperation(problem);
  }
  return { part1, part1};
}
}

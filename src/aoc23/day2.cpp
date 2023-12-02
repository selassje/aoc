#include "aoc23/day2.hpp"
#include <iostream>

namespace aoc23::day2 {
Result
solve(const Input& input)
{
  for (const auto& game : input) {
    for (const auto& draw : game) {
      std::cout << draw.blueCount << "\n";
    }
  }
  return { input.size(), 0 };
}
}
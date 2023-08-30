#include "day4.hpp"

#include <algorithm>

namespace aoc22::day4 {

Result
solve(const Input& input)
{
  auto first_contains_second = [](const Range& r1, const Range& r2) {
    return r1.first <= r2.first && r1.second >= r2.second;
  };

  auto one_contains_another = [f = first_contains_second](const Range& r1,
                                                          const Range& r2) {
    return f(r1, r2) || f(r2, r1);
  };

  auto are_exclusive = [](const Range& r1, const Range& r2) {
    return r1.second < r2.first || r2.second < r1.first;
  };
  
  const auto result_part1 = static_cast<unsigned int>(
    std::ranges::count_if(input, [f = one_contains_another](const Pair& p) {
      return f(p.first, p.second);
    }));
  
  const auto result_part2 = static_cast<unsigned int>(
    input.size() - std::ranges::count_if(input, [f = are_exclusive](const Pair& p) {
      return f(p.first, p.second);
    }));

  return { result_part1, result_part2 };
}

};
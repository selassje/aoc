#include "day4.hpp"

#include <algorithm>
#include <functional>

namespace aoc22::day4 {

Result
solve(const Input& input)
{
  const auto first_contains_second = [](const Range& r1, const Range& r2) {
    return r1.first <= r2.first && r1.second >= r2.second;
  };

  const auto one_contains_another =
    [f = first_contains_second](const Range& r1, const Range& r2) {
      return f(r1, r2) || f(r2, r1);
    };

  auto are_exclusive = [](const Range& r1, const Range& r2) {
    return r1.second < r2.first || r2.second < r1.first;
  };

  auto solve_internal = [&input](auto f) {
    return std::ranges::count_if(
      input, [f](const Pair& p) { return f(p.first, p.second); });
  };

  const auto result_part1 =
    static_cast<unsigned int>(solve_internal(one_contains_another));

  const auto result_part2 =
    static_cast<unsigned int>(input.size()) -
    static_cast<unsigned int>(solve_internal(are_exclusive));

  return { result_part1, result_part2 };
}

};
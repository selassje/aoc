#include "day4.hpp"

#include <algorithm>
#include <functional>

namespace aoc22::day4 {

Result
solve(const Input& input)
{
  const auto firstContainsSecond = [](const Range& r1, const Range& r2) {
    return r1.first <= r2.first && r1.second >= r2.second;
  };

  const auto oneContainsAnother = [f = firstContainsSecond](const Range& r1,
                                                            const Range& r2) {
    return f(r1, r2) || f(r2, r1);
  };

  auto areExclusive = [](const Range& r1, const Range& r2) {
    return r1.second < r2.first || r2.second < r1.first;
  };

  auto solveInternal = [&input](auto f) {
    return std::ranges::count_if(
      input, [f](const Pair& p) { return f(p.first, p.second); });
  };

  const auto resultPart1 =
    static_cast<unsigned int>(solveInternal(oneContainsAnother));

  const auto resultPart2 =
    static_cast<unsigned int>(input.size()) -
    static_cast<unsigned int>(solveInternal(areExclusive));

  return { resultPart1, resultPart2 };
}

};
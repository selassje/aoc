#include "aoc24/day2.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iterator>
#include <ranges>
#include <vector>

namespace {

using List = std::vector<std::uint32_t>;
bool
isSafe(const List& list)
{
  auto diffs = std::views::zip(list, list | std::views::drop(1)) |
               std::views::transform([](const auto& pair) {
                 const auto first = std::get<0>(pair);
                 const auto second = std::get<1>(pair);
                 if (first > second) {
                   return static_cast<std::int64_t>(first - second);
                 }
                 return -static_cast<std::int64_t>(second - first);
               });
  const auto isDecreasing =
    std::ranges::all_of(diffs, [](const auto& value) { return value > 0; });
  const auto isIncreasing =
    std::ranges::all_of(diffs, [](const auto& value) { return value < 0; });
  if (isDecreasing || isIncreasing) {
    return std::ranges::all_of(
      diffs, [](const auto& value) { return std::abs(value) <= 3; });
  }
  return false;
}

bool
isSafeWithRemoval(const List& list)
{
  if (isSafe(list)) {
    return true;
  }

  for (std::size_t i = 0; i < list.size(); ++i) {
    auto cpList = list;
    auto it = cpList.begin();
    std::advance(it, i);
    cpList.erase(it);
    if (isSafe(cpList)) {
      return true;
    }
  }
  return false;
}

}

namespace aoc24::day2 {
[[nodiscard]] Result
solve(const Input& input) noexcept
{
  const auto part1 =
    static_cast<std::size_t>(std::ranges::count_if(input, isSafe));
  const auto part2 =
    static_cast<std::size_t>(std::ranges::count_if(input, isSafeWithRemoval));
  return { part1, part2 };
}

}
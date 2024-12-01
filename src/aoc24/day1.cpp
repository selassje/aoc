#include "aoc24/day1.hpp"

#include <algorithm>
#include <cstdint>
#include <functional>
#include <ranges>
#include <vector>

namespace aoc24::day1 {

Result
solve(Input input)
{
  auto getDistance = [](const auto& pair) {
    if (pair.first >= pair.second) {
      return pair.first - pair.second;
    }
    return pair.second - pair.first;
  };

  auto leftList = input | std::views::keys | std::ranges::to<std::vector>();
  std::ranges::sort(leftList);
  auto rightList = input | std::views::values | std::ranges::to<std::vector>();
  std::ranges::sort(rightList);
  auto distances =
    std::views::zip(leftList, rightList) | std::views::transform(getDistance);
  const auto part1 = std::ranges::fold_left(distances,0, std::plus<>());

  return { part1, part1 };
}

}
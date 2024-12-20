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
    const auto first = std::get<0>(pair);
    const auto second = std::get<1>(pair);
    if (first >= second) {
      return first - second;
    }
    return second - first;
  };

  auto leftList = input | std::views::keys | std::ranges::to<std::vector>();
  std::ranges::sort(leftList);
  auto rightList = input | std::views::values | std::ranges::to<std::vector>();
  std::ranges::sort(rightList);
  auto distances =
    std::views::zip(leftList, rightList) | std::views::transform(getDistance);
  const auto part1 =
    std::ranges::fold_left(distances, 0, std::plus<>()); // NOLINT

  auto similarities =
    leftList | std::views::transform([&rightList](const auto& value) {
      return value *
             static_cast<std::uint32_t>(std::ranges::count(rightList, value));
    });

  const auto part2 =
    std::ranges::fold_left(similarities, 0, std::plus<>()); // NOLINT
  return { part1, part2 };
}

}
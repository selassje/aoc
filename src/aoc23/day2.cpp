#include "aoc23/day2.hpp"
#include <algorithm>
#include <cstddef>

namespace aoc23::day2 {

namespace {
std::size_t
getMininumSetPower(const Game& game)
{
  Draw minSet{};
  for (const auto& draw : game) {
    minSet.redCount = std::max(minSet.redCount, draw.redCount);
    minSet.greenCount = std::max(minSet.greenCount, draw.greenCount);
    minSet.blueCount = std::max(minSet.blueCount, draw.blueCount);
  }
  return minSet.redCount * minSet.greenCount * minSet.blueCount;
}
}

Result
solve(const Input& input)
{
  static constexpr Draw maxDraw = { 12, 13, 14 };
  auto isDrawValid = [](const Draw& draw) {
    return draw.redCount <= maxDraw.redCount &&
           draw.greenCount <= maxDraw.greenCount &&
           draw.blueCount <= maxDraw.blueCount;
  };

  std::size_t part1 = 0;
  std::size_t part2 = 0;
  for (std::size_t i = 0; i < input.size(); ++i) {
    const auto& game = input[i];
    if (std::ranges::count_if(
          game, [&](const auto& draw) { return !isDrawValid(draw); }) == 0) {
      part1 += i + 1;
    }
    part2 += getMininumSetPower(game);
  }
  return { part1, part2 };
}
}
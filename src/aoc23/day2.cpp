#include "aoc23/day2.hpp"
#include <algorithm>
#include <iostream>

namespace aoc23::day2 {

struct DrawEx
{
  std::size_t redCount;
  std::size_t greenCount;
  std::size_t blueCount;

  constexpr DrawEx(const Draw& draw)
    : redCount{ draw.redCount }
    , greenCount{ draw.greenCount }
    , blueCount{ draw.blueCount } {};

  constexpr DrawEx(std::initializer_list<std::size_t> list)
  {
    auto it = list.begin(); // NOLINT
    redCount = *it++;
    greenCount = *it++;
    blueCount = *it;
  }
  constexpr auto operator<=>(const DrawEx&) const noexcept = default;
};

bool
isDrawValid(const Draw& limit, const Draw& draw)
{
  return draw.redCount <= limit.redCount &&
         draw.greenCount <= limit.greenCount &&
         draw.blueCount <= limit.blueCount;
}

Result
solve(const Input& input)
{
  static constexpr Draw maxDraw = { 12, 13, 14 };
  std::size_t part1 = input.size();
  part1 = 0;
  for (std::size_t i = 0; i < input.size(); ++i) {
    const auto& game = input[i];
    if (std::ranges::count_if(game, [&](const auto& draw) {
          return !isDrawValid(maxDraw, draw);
        }) == 0) {
      part1 += i + 1;
    } else {
      std::cout << "ERROR " << i + 1 << "\n";
    }
  }
  return { part1, part1 };
}
}
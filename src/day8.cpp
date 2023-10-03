#include "day8.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <utility>

namespace aoc22::day8 {

std::pair<std::size_t, std::size_t>
solve(const Grid& grid)
{
  const std::size_t height = grid.size();
  const std::size_t width = grid[0].size();
  const std::size_t edges = 2 * height + 2 * (width - 2);

  enum class Direction
  {
    Left,
    Right,
    Up,
    Down
  };
  using enum Direction;

  struct alignas(32) SearchPattern
  {
    std::int32_t incH;
    std::int32_t incW;
    std::size_t endH;
    std::size_t endW;
  };
  // clang-format off
  auto getSearchPattern =
    [height, width](
      std::size_t h, std::size_t w, const Direction& dir) -> SearchPattern { //NOLINT
    switch (dir) {
      case Left:
        return { 0, -1, h, 0 };
      case Right:
        return { 0, 1, h, width - 1 };
      case Up:
        return { -1, 0, 0, w };
      case Down:
        return { 1, 0, height - 1, w };
    };
    return {};
  };
  // clang-format on

  std::size_t innerVisible = 0;
  std::size_t resultPart2 = 0;
  for (std::size_t h = 1; h < height - 1; ++h) {
    for (std::size_t w = 1; w < width - 1; ++w) {
      auto scenicScoreAndVisibility =
        [w, h, &grid, &getSearchPattern](Direction dir) {
          auto hCurr = h;
          auto wCurr = w;
          const auto tree = grid[hCurr][wCurr];
          auto searchPattern = getSearchPattern(hCurr, wCurr, dir);
          std::size_t score = 0;
          bool isVisible = true;
          while (searchPattern.endH != hCurr || searchPattern.endW != wCurr) {
            hCurr += static_cast<std::size_t>(searchPattern.incH);
            wCurr += static_cast<std::size_t>(searchPattern.incW);
            ++score;
            if (grid[hCurr][wCurr] >= tree) {
              isVisible = false;
              break;
            }
          }
          return std::make_pair(score, isVisible);
        };

      bool isVisible = false;
      std::size_t score = 1;
      for (std::size_t i = 0; i < 4; ++i) {
        const auto& [score_, isVisible_] =
          scenicScoreAndVisibility(static_cast<Direction>(i));
        score *= score_;
        isVisible |= isVisible_;
      }

      if (isVisible) {
        ++innerVisible;
      }
      resultPart2 = std::max(score, resultPart2);
    }
  }

  const std::size_t resultPart1 = edges + innerVisible;
  return std::make_pair(resultPart1, resultPart2);
}

};
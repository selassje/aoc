#include "day8.hpp"

#include <cstdint>
#include <numeric>

namespace aoc22::day8 {

std::pair<std::size_t, std::size_t>
solve(const Grid& grid)
{
  const size_t height = grid.size();
  const size_t width = grid[0].size();

  Grid maxesToTheLeft{};
  Grid maxesToTheRight{};

  for (std::size_t h = 0; h < height; ++h) {
    std::vector<std::byte> maxToTheLeft{};
    maxToTheLeft.resize(width);
    maxToTheLeft[0] = grid[h][0];
    for (std::size_t w = 1; w < width; ++w) {
      maxToTheLeft[w] = std::max(maxToTheLeft[w - 1], grid[h][w]);
    }
    std::vector<std::byte> maxToTheRight{};
    maxToTheRight.resize(width);
    maxToTheRight[width - 1] = grid[h][width - 1];
    for (std::size_t w = width - 2; w != static_cast<std::size_t>(-1); --w) {
      maxToTheRight[w] = std::max(maxToTheRight[w + 1], grid[h][w]);
      if (w == 0)
        break;
    }
    maxesToTheLeft.push_back(maxToTheLeft);
    maxesToTheRight.push_back(maxToTheRight);
  }
  Grid maxesUp{};
  Grid maxesDown{};

  for (std::size_t w = 0; w < width; ++w) {
    std::vector<std::byte> maxUp{};
    maxUp.resize(height);
    maxUp[0] = grid[0][w];
    for (std::size_t h = 1; h < height; ++h) {
      maxUp[h] = std::max(maxUp[h - 1], grid[h][w]);
    }
    std::vector<std::byte> maxDown{};
    maxDown.resize(height);
    maxDown[height - 1] = grid[height - 1][w];
    for (std::size_t h = height - 2; h < static_cast<std::size_t>(-1); --h) {
      maxDown[h] = std::max(maxDown[h + 1], grid[h][w]);
    }
    maxesUp.push_back(maxUp);
    maxesDown.push_back(maxDown);
  }

  const std::size_t edges = 2 * height + 2 * (width - 2);
  std::size_t innerVisible = 0;

  for (std::size_t h = 1; h < height - 1; ++h)
    for (std::size_t w = 1; w < width - 1; ++w) {
      const auto& tree = grid[h][w];
      if (tree > maxesToTheLeft[h][w - 1] || tree > maxesToTheRight[h][w + 1] ||
          tree > maxesUp[w][h - 1] || tree > maxesDown[w][h + 1]) {
        ++innerVisible;
      }
    }

  enum class Direction
  {
    Left,
    Right,
    Up,
    Down
  };
  using enum Direction;

  struct SearchPattern
  {
    std::int32_t incH;
    std::int32_t incW;
    std::size_t endH;
    std::size_t endW;
  };

  auto getSearchPattern =
    [height, width](
      std::size_t h, std::size_t w, const Direction& dir) -> SearchPattern {
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

  std::size_t resultPart2 = 0;
  for (std::size_t _h = 0; _h < height; ++_h)
    for (std::size_t _w = 0; _w < width; ++_w) {
      auto scenicScore =
        [&grid, getSearchPattern](Direction dir, std::size_t h, std::size_t w) mutable {
          const auto tree = grid[h][w];
          auto searchPattern = getSearchPattern(h, w, dir);
          std::size_t score = 0;
          while (searchPattern.endH != h || searchPattern.endW != w) {
            h += static_cast<std::size_t>(searchPattern.incH);
            w += static_cast<std::size_t>(searchPattern.incW);
            ++score;
            if (grid[h][w] >= tree) {
              break;
            }
          }
          return score;
        };
      const auto score = scenicScore(Up,_h,_w) * scenicScore(Left,_h,_w) *
                         scenicScore(Right,_h,_w) * scenicScore(Down,_h,_w);
      resultPart2 = std::max(score, resultPart2);
    }
  const size_t resultPart1 = edges + innerVisible;

  return std::make_pair(resultPart1, resultPart2);
}

};
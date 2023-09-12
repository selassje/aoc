#include "day8.hpp"

#include <numeric>

namespace aoc22::day8 {

std::pair<std::size_t, std::size_t>
solve(const Grid& grid)
{
  const size_t height = grid.size();
  const size_t width = grid[0].size();

  Grid maxesToTheLeft{};
  Grid maxesToTheRight{};

  for ( std::size_t h = 0 ; h < height  ; ++h)
  {
      std::vector<std::byte> maxToTheLeft{};
      maxToTheLeft.reserve(width);
      maxToTheLeft[0] = grid[h][0];
      for ( std::size_t w = 1 ; w < width; ++w)
      {
        maxToTheLeft[w] = std::max(maxToTheLeft[w-1], grid[h][w]);
      }
      std::vector<std::byte> maxToTheRight{};
      maxToTheRight.reserve(width);
      maxToTheRight[width - 1] = grid[h][width - 1];
      for ( std::size_t w = width - 2 ; w >= 0; --w)
      {
        maxToTheRight[w] = std::max(maxToTheRight[w + 1], grid[h][w]);
      }
      maxesToTheLeft.push_back(maxToTheLeft);
      maxesToTheRight.push_back(maxToTheRight);
  }
  Grid maxesUp{};
  Grid maxesDown{};
  
  for ( std::size_t w = 0 ; w < width  ; ++w)
  {
      std::vector<std::byte> maxUp{};
      maxUp.reserve(height);
      maxUp[0] = grid[0][w];
      for ( std::size_t h = 1 ; h < height; ++h)
      {
        maxUp[h] = std::max(maxUp[h-1], grid[h][w]);
      }
      std::vector<std::byte> maxDown{};
      maxDown.reserve(height);
      maxDown[height - 1] = grid[height - 1][w];
      for ( std::size_t h = height - 2 ; h >= 0; --h)
      {
        maxDown[h] = std::max(maxDown[h + 1], grid[h][w]);
      }
      maxesUp.push_back(maxUp);
      maxesDown.push_back(maxDown);
  }

  const std::size_t edges = 2 * height + 2 * ( width - 2);
  std::size_t innerVisible = 0;

  for ( std::size_t h = 1 ; h < height - 1 ; ++h)
    for ( std::size_t w = 1 ; w < width - 1 ; ++w)
    {
        const auto &tree = grid[h][w];
        if ( tree > maxesToTheLeft[h][w - 1] ||
             tree > maxesToTheRight[h][w + 1] ||
             tree > maxesUp[w][h - 1]  ||
             tree > maxesDown[w][h + 1])
        {
            ++innerVisible;
        }
    }
  const size_t resultPart1 = edges + innerVisible;

  return std::make_pair(resultPart1, resultPart1);
}

};
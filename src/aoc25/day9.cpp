module aoc25.day9;

import aoc.matrix;

using aoc25::day9::Input;
using aoc25::day9::Point;

enum class Tile : std::uint8_t
{
  Empty,
  Red,
  Green
};
using Matrix = aoc::matrix::Matrix<Tile>;
using Points = std::vector<Point>;

auto
getArea(const Point& a, const Point& b)
{
  return (std::max(a.x, b.x) - std::min(a.x, b.x) + 1) *
         (std::max(a.y, b.y) - std::min(a.y, b.y) + 1);
}
struct Wall
{
  Point start;
  Point end;
};

using Walls = std::vector<Wall>;


namespace {

bool
isOnWall(const Point& p, const Wall& wwall)
{
  if (wwall.start.x == wwall.end.x) {
    // Vertical line
    if (p.x != wwall.start.x) {
      return false;
    }
    return (p.y >= std::min(wwall.start.y, wwall.end.y) &&
            p.y <= std::max(wwall.start.y, wwall.end.y));
  }
  if (wwall.start.y == wwall.end.y) {
    // Horizontal line
    if (p.y != wwall.start.y) {
      return false;
    }
    return (p.x >= std::min(wwall.start.x, wwall.end.x) &&
            p.x <= std::max(wwall.start.x, wwall.end.x));
  }
  return false;
}

bool
isOnAnyWall(const Point& p, const Walls& walls)
{
  for (const auto& wall : walls) {
    if (isOnWall(p, wall)) {
      return true;
    }
  }
  return false;
}

// Points auto

bool
isInside2(const Point& p, const Walls& walls)
{
  if( isOnAnyWall(p, walls)) {
    return true;
  }
  if(p.x == 0) {
    return false;
  }

  std::uint64_t lastWallX =  0;
  std::size_t wallsCrossed = 0;
  for (std::size_t i = 0; i < p.x; ++i) {
    const auto x = p.x - i - 1;
    const auto point = Point{ x, p.y };
    if (isOnAnyWall(point, walls)) {
      if(wallsCrossed == 0) {
        ++wallsCrossed;
      } else if( lastWallX - 1 != x) {
        ++wallsCrossed;
      }
      lastWallX = x;
    }
  }
  return (wallsCrossed % 2) == 1;
}
/*
bool
isInside(const Point& p,
         const Matrix& grid,
         std::uint64_t minX,
         std::uint64_t minY)
{
  auto access = [&](auto x, auto y) { return grid[x - minX, y - minY]; };
  bool foundWallLeft = false;
  for (std::size_t i = 0; i <= p.x; ++i) {
    const auto x = p.x - i;
    const auto tile = access(x, p.y);
    if (tile == Tile::Red || tile == Tile::Green) {
      foundWallLeft = true;
      break;
    }
  }
  bool foundWallRight = false;
  for (std::size_t x = p.x; x < grid.width(); ++x) {
    const auto tile = access(x, p.y);
    if (tile == Tile::Red || tile == Tile::Green) {
      foundWallRight = true;
      break;
    }
  }
  bool foundWallTop = false;
  for (std::size_t i = 0; i <= p.y; ++i) {
    const auto y = p.y - i;
    const auto tile = access(p.x, y);
    if (tile == Tile::Red || tile == Tile::Green) {
      foundWallTop = true;
      break;
    }
  }
  bool foundWallBottom = false;
  for (std::size_t y = p.y; y < grid.height(); ++y) {
    const auto tile = access(p.x, y);
    if (tile == Tile::Red || tile == Tile::Green) {
      foundWallBottom = true;
      break;
    }
  }
  return foundWallLeft && foundWallRight && foundWallTop && foundWallBottom;
}
*/
auto
buildGrid(const Input& input)
{
  std::uint64_t minX = 0;
  std::uint64_t maxX = 0;
  std::uint64_t minY = 0;
  std::uint64_t maxY = 0;
  for (std::size_t i = 0; i < input.size(); ++i) {
    for (std::size_t j = i + 1; j < input.size(); ++j) {
      const auto& a = input[i];
      const auto& b = input[j];
      minX = std::min({ minX, a.x, b.x });
      maxX = std::max({ maxX, a.x, b.x });
      minY = std::min({ minY, a.y, b.y });
      maxY = std::max({ maxY, a.y, b.y });
    }
  }

  Matrix grid({ maxX - minX + 1, maxY - minY + 1 }, Tile::Empty);

  auto access = [&](Point p) -> Tile& { return grid[p.x - minX, p.y - minY]; };

  Walls walls{};
  for (std::size_t i = 0; i < input.size(); ++i) {
    const auto& redTilePoint = input[i];
    const auto& nextRedTilePoint = input[(i + 1) % input.size()];
    access(redTilePoint) = Tile::Red;
    access(nextRedTilePoint) = Tile::Red;

    const std::uint64_t startX = std::min(redTilePoint.x, nextRedTilePoint.x);
    const std::uint64_t endX = std::max(redTilePoint.x, nextRedTilePoint.x);
    const std::uint64_t startY = std::min(redTilePoint.y, nextRedTilePoint.y);
    const std::uint64_t endY = std::max(redTilePoint.y, nextRedTilePoint.y);

    if (endX - startX == 0) {
      for (std::uint64_t y = startY; y <= endY; ++y) {
        if (access({ startX, y }) != Tile::Red) {
          access({ startX, y }) = Tile::Green;
        }
      }
    } else if (endY - startY == 0) {
      for (std::uint64_t x = startX; x <= endX; ++x) {
        if (access({ x, startY }) != Tile::Red) {
          access({ x, startY }) = Tile::Green;
        }
      }
    }
    walls.push_back(Wall{ redTilePoint, nextRedTilePoint });
  }

  std::uint64_t maxArea = 0;
  for (std::size_t i = 0; i < input.size(); ++i) {
    for (std::size_t j = i + 1; j < input.size(); ++j) {
      const auto bottomLeft = Point{ std::min(input[i].x, input[j].x),
                                     std::min(input[i].y, input[j].y) };
      const auto topRight = Point{ std::max(input[i].x, input[j].x),
                                   std::max(input[i].y, input[j].y) };

      bool isValid = true;
      for (std::uint64_t x = bottomLeft.x; x <= topRight.x; ++x) {
        for (std::uint64_t y = bottomLeft.y; y <= topRight.y; ++y) {
          if (!isInside2(Point{ x, y }, walls)) {
            isValid = false;
            break;
          }
        }
      }
      if (isValid) {
        maxArea = std::max(getArea(input[i], input[j]), maxArea);
      }
    }
  }
  std::print("Max area rectangle: {}\n", maxArea);
  return grid;
}
}
namespace aoc25::day9 {

Result
solve(const Input& input)
{
  std::uint64_t part1 = 0;

  for (std::size_t i = 0; i < input.size(); ++i) {
    for (std::size_t j = i + 1; j < input.size(); ++j) {
      part1 = std::max(getArea(input[i], input[j]), part1);
    }
  }

  const auto grid = buildGrid(input);
  /*
    auto print = [](Tile t) {
      switch (t) {
        case Tile::Empty:
          return '.';
        case Tile::Red:
          return '#';
        case Tile::Green:
          return 'x';
      }
      return '?';
    };
  */
  // grid.print(print);
  std::uint64_t part2 = part1;
  return { part1, part2 };
}

}

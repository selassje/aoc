
#include "aoc24/day12.hpp"
#include <algorithm>
#include <compare>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <ranges>
#include <set>
#include <tuple>
#include <vector>

namespace aoc24::day12 {

namespace {

struct Point
{
  std::size_t x;
  std::size_t y;
  constexpr std::strong_ordering operator<=>(const Point&) const = default;
};

struct Size
{
  std::size_t height;
  std::size_t width;
};

struct Region
{
  std::vector<Point> points;
  std::size_t perimeter;
  std::size_t sides;
};

auto
getNeighbours(Point p, Size size)
{
  std::vector<Point> neighbours{};
  const auto [x, y] = p;
  const auto [height, width] = size;
  if (x > 0) {
    neighbours.emplace_back(x - 1, y);
  }
  if (x < width - 1) {
    neighbours.emplace_back(x + 1, y);
  }
  if (y > 0) {
    neighbours.emplace_back(x, y - 1);
  }
  if (y < height - 1) {
    neighbours.emplace_back(x, y + 1);
  }
  return neighbours;
}

auto
getTotalPrice(const std::vector<Region>& regions)
{
  namespace rng = std::ranges;
  using pair = std::pair<std::size_t, std::size_t>;
  auto prices =
    regions | rng::views::transform([](const auto& r) {
      return pair{ r.perimeter * r.points.size(), r.sides * r.points.size() };
    });
  return rng::fold_left( // NOLINT
    prices,
    pair{ 0ULL, 0ULL },
    [](const pair& p1, const pair& p2) {
      return pair{ p1.first + p2.first, p1.second + p2.second };
    });
}

bool arePointsTouching(const Point &p1, const Point &p2) {
    const auto xDiff = std::max(p1.x,p2.x)  - std::min(p1.x,p2.x);
    const auto yDiff = std::max(p1.y,p2.y)  - std::min(p1.y,p2.y);
    return (xDiff == 0 && yDiff == 1) || (xDiff == 1 && yDiff == 0);
}

enum class Corner : std::uint8_t {
    TopLeft = 0,
    TopRight = 1,
    BottomRight = 2,
    BottomLeft = 3
};

struct PointEx {
  std::size_t x;
  std::size_t y;
  Corner corner;
  constexpr std::strong_ordering operator<=>(const PointEx&) const = default;
};
bool arePointsExEqual(const PointEx &p1, const PointEx &p2) {
    const auto xDiff = std::max(p1.x,p2.x)  - std::min(p1.x,p2.x);
    const auto yDiff = std::max(p1.y,p2.y)  - std::min(p1.y,p2.y);
    if ( xDiff == 0 && yDiff == 0) {
      return p1.corner == p2.corner;
    }

    if( xDiff == 0 && yDiff == 1) {
      const auto top = p1.y < p2.y ? p1 : p2;
      const auto bottom = p1.y < p2.y ? p2 : p1;

      return (top.corner == Corner::BottomRight && bottom.corner == Corner::TopRight) || 
        (top.corner == Corner::BottomLeft && bottom.corner == Corner::TopLeft);
    }
    
    if( xDiff == 1 && yDiff == 0) {
      const auto left = p1.x < p2.x ? p1 : p2;
      const auto right = p1.x < p2.x ? p2 : p1;

      return (left.corner == Corner::BottomRight && right.corner == Corner::BottomLeft) || 
        (left.corner == Corner::TopRight && right.corner == Corner::TopLeft);
    }
    return false;
}

bool isTurn(const PointEx &p1, const PointEx &p2) {
    const auto xDiff = std::max(p1.x,p2.x)  - std::min(p1.x,p2.x);
    const auto yDiff = std::max(p1.y,p2.y)  - std::min(p1.y,p2.y);
    return xDiff == 1 && yDiff == 1;
}

auto
getNeighbours2(PointEx p, const Input &input)
{
  const auto size = Size{ input.size(), input[0].size() };
  std::vector<PointEx> neighbours{};
  return neighbours;
}

std::tuple<PointEx,PointEx> getSide(Point p, const Input& input) {

  return {};
}

auto getSides(const std::vector<Point> edges, const Input& input) {
    const auto edgeCount = edges.size();
    std::size_t sides = 1;
    auto [last_, last ] = getSide( edges[0], input);
    const auto startPoint = last_;
    
    while(true) {
        PointEx next{};
        for(const auto n : getNeighbours2(last, input)) {
          if( !arePointsExEqual(n, last_)) {
            next = n;
            break;
          }
        }
        if(arePointsExEqual(next, startPoint)) {
            break;
        }
        if(isTurn(last_, next) ) {
            ++sides;
        }
        last_ = last;
        last = next;

    }
    return  sides;
}


auto
getRegion(Point start, const Input& input)
{
  std::size_t perimeter = 0;
  const auto size = Size{ input.size(), input[0].size() };
  const auto plant = input[start.y][start.x];
  std::deque<Point> toBeVisitedPoints{ start };
  std::set<Point> visitedPoints{};
  std::vector<Point> edges{};

  while (!toBeVisitedPoints.empty()) {
    const auto point = toBeVisitedPoints.front();
    toBeVisitedPoints.pop_front();
    const auto neighbours = getNeighbours(point, size);
    std::size_t perimeterToAdd = 4 - neighbours.size();
    for (const auto& neighbour : neighbours) {
      if (input[neighbour.y][neighbour.x] == plant) {
        if (!visitedPoints.contains(neighbour)) {
          if (std::ranges::find(toBeVisitedPoints, neighbour) ==
              std::end(toBeVisitedPoints)) {
            toBeVisitedPoints.push_back(neighbour);
          }
        }
      } else {
        ++perimeterToAdd;
      }
    }
    perimeter += perimeterToAdd;
    if(perimeterToAdd > 0) {
      edges.push_back(point);
    }
    visitedPoints.insert(point);
  }
  return Region{ { std::begin(visitedPoints), std::end(visitedPoints) },
                 perimeter,
                 getSides(edges,input)};
}

}

Result
solve(const Input& input)
{
  std::vector<Region> regions{};
  namespace rng = std::ranges;
  const auto size = Size{ input.size(), input[0].size() };
  for (std::size_t y = 0; y < size.height; ++y) {
    for (std::size_t x = 0; x < size.width; ++x) {
      const Point p = { x, y };
      if (rng::none_of(
            regions,
            [&p](const auto& points) {
              return rng::find(points, p) != std::end(points);
            },
            &Region::points)) {
        regions.push_back(getRegion(p, input));
      }
    }
  }
  const auto [part1, part2] = getTotalPrice(regions);
  return { part1, part2 };
}

}
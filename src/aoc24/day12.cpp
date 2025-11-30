
#include "aoc22/day10.hpp"
#include "aoc24/day12.hpp"
#include <algorithm>
#include <cassert>
#include <compare>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <optional>
#include <print>
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

bool
arePointsTouching(const Point& p1, const Point& p2)
{
  const auto xDiff = std::max(p1.x, p2.x) - std::min(p1.x, p2.x);
  const auto yDiff = std::max(p1.y, p2.y) - std::min(p1.y, p2.y);
  return (xDiff == 0 && yDiff == 1) || (xDiff == 1 && yDiff == 0);
}

enum class Corner : std::uint8_t
{
  TopLeft = 0,
  TopRight = 1,
  BottomRight = 2,
  BottomLeft = 3
};

// struct PointEx {
//  std::size_t x;
// std::size_t y;
// Corner corner;
// constexpr std::strong_ordering operator<=>(const PointEx&) const = default;
//};
using PointEx = Point;
/*
bool arePointsExEqual(const PointEx &p1, const PointEx &p2) {
    const auto xDiff = std::max(p1.x,p2.x)  - std::min(p1.x,p2.x);
    const auto yDiff = std::max(p1.y,p2.y)  - std::min(p1.y,p2.y);
    if ( xDiff == 0 && yDiff == 0) {
      return p1.corner == p2.corner;
    }

    if( xDiff == 0 && yDiff == 1) {
      const auto top = p1.y < p2.y ? p1 : p2;
      const auto bottom = p1.y < p2.y ? p2 : p1;

      return (top.corner == Corner::BottomRight && bottom.corner ==
Corner::TopRight) || (top.corner == Corner::BottomLeft && bottom.corner ==
Corner::TopLeft);
    }

    if( xDiff == 1 && yDiff == 0) {
      const auto left = p1.x < p2.x ? p1 : p2;
      const auto right = p1.x < p2.x ? p2 : p1;

      return (left.corner == Corner::BottomRight && right.corner ==
Corner::BottomLeft) || (left.corner == Corner::TopRight && right.corner ==
Corner::TopLeft);
    }
    return false;
}
*/

bool
areVertical(PointEx p1, PointEx p2)
{
  const auto xDiff = std::max(p1.x, p2.x) - std::min(p1.x, p2.x);
  const auto yDiff = std::max(p1.y, p2.y) - std::min(p1.y, p2.y);
  return xDiff == 0;
}

bool
isTurn(const PointEx& p1, const PointEx& p2, const PointEx& p3)
{
  return areVertical(p1, p2) != areVertical(p2, p3);
}

bool
isCorner(PointEx p, const Input& input, unsigned char plant)
{
  std::size_t borderingPlants = 0;
  const auto size = Size{ input.size(), input[0].size() };
  auto getPlant = [&](const PointEx point) { return input[point.y][point.x]; };
  if (p.x < size.width && p.y < size.height) {
    if( plant == getPlant(p)) {
      ++borderingPlants;
    }
  }
  if (p.x < size.width && p.y > 0) {
    PointEx rightTop = { p.x, p.y - 1 };
    if( plant == getPlant(rightTop)) {
      ++borderingPlants;
    }
  }
  if (p.x > 0 && p.y < size.width) {
    PointEx leftBottom = { p.x - 1, p.y };
    if( plant == getPlant(leftBottom)) {
      ++borderingPlants;
    }
  }
  if (p.x > 0 && p.y > 0) {
    PointEx leftTop = { p.x - 1, p.y - 1 };
    if( plant == getPlant(leftTop)) {
      ++borderingPlants;
    }
  }
  return borderingPlants > 0 && borderingPlants < 4;
}

std::tuple<PointEx, PointEx>
getSide(Point p, const Input& input, const std::set<Point> corners)
{
  const auto plant = input[p.y][p.x];
  const auto leftTop = p;
  const auto rightTop = PointEx{ p.x + 1, p.y };
  const auto leftBottom = PointEx{ p.x, p.y + 1 };
  const auto rightBottom = PointEx{ p.x + 1, p.y + 1 };

  auto inRegion = [&](const PointEx point) { 
    //return true;
    return std::find(corners.begin(),corners.end(),point) != corners.end(); 
    };
  if (isCorner(leftTop, input, plant) && isCorner(rightTop, input, plant)) {
    if(inRegion(leftTop) && inRegion(rightTop))
    return { leftTop, rightTop };
  }
  if (isCorner(leftTop, input, plant) && isCorner(leftBottom, input, plant)) {
    if(inRegion(leftTop) && inRegion(leftBottom))
    return { leftTop, leftBottom };
  }

  if (isCorner(leftBottom, input, plant) &&
      isCorner(rightBottom, input, plant)) {
    if(inRegion(leftBottom) && inRegion(rightBottom))
    return { leftBottom, rightBottom };
  }

  if (isCorner(rightTop, input, plant) && isCorner(rightBottom, input, plant)) {
    if(inRegion(rightTop) && inRegion(rightBottom)) {
    return { rightTop, rightBottom };
}
  }

  return {};
}
auto
getNeighbours2(PointEx p,PointEx last,  const Input& input, unsigned char plant, const std::vector<Point>& edges, const std::set<Point> corners)
{
  const bool fromRight = last.x > p.x;
  const bool fromLeft = last.x < p.x;
  const bool fromUp = last.y < p.y;
  const bool fromDown = last.y >  p.y;
  const bool horizontal = fromRight || fromLeft;
  const bool vertical = fromUp || fromDown;

  if(p == last) {
  //  const auto [p1,p2] = getSide(p, input,corners); 
  //  return std::vector{p1,p2};
  }
  bool firstNeighbours = p == last;
  firstNeighbours = false;

  const auto size = Size{ input.size(), input[0].size() };
  std::vector<PointEx> neighbours{};
  auto getPlant = [&](const PointEx point) { return input[point.y][point.x]; };
  auto inRegion = [&](const PointEx point) { 
    return true;
    return std::find(edges.begin(),edges.end(),point) != edges.end(); 
    };
  auto isInside = [&](const PointEx point) { 
    return point.x < size.width && point.y < size.height; 
    };
  
  std::size_t squares = 0;
  std::size_t samePlantsTotal = 0;
  std::optional<PointEx> topLeft = std::nullopt;
  std::optional<unsigned char> topLeftPlant = std::nullopt;
  if ( p.x > 0 && p.y > 0) {
    const auto t = PointEx{p.x - 1, p.y -1};
    if(isInside(t)) {
      topLeft = t;
      topLeftPlant = getPlant(t);
      ++squares;
      if( plant == *topLeftPlant) {
        ++samePlantsTotal;
      }
    }

  }
  std::optional<PointEx> topRight = std::nullopt;
  std::optional<unsigned char> topRightPlant = std::nullopt;
  if ( p.y > 0) {
    const auto t = PointEx{p.x, p.y -1};
    if(isInside(t)) {
        topRight = t;
      ++squares;
      topRightPlant = getPlant(t);
      if( plant == *topRightPlant) {
        ++samePlantsTotal;
      }

    }
  }
  std::optional<PointEx> bottomRight = std::nullopt;
  std::optional<unsigned char> bottomRightPlant = std::nullopt;
  if ( isInside(p)) {
    bottomRight = PointEx{p.x, p.y};
    ++squares;
      bottomRightPlant = getPlant(*bottomRight);
      if( plant == *bottomRightPlant) {
        ++samePlantsTotal;
      }
  }

  std::optional<PointEx> bottomLeft = std::nullopt;
  std::optional<unsigned char> bottomLeftPlant = std::nullopt;
  if ( p.x > 0) {
    const auto t = PointEx{p.x - 1, p.y};
    if ( isInside(p)) {
      bottomLeft = t;
      ++squares;
      bottomLeftPlant = getPlant(t);
      if( plant == *bottomLeftPlant) {
        ++samePlantsTotal;
      }
    }
  }

  bool isDiagonal = samePlantsTotal == 2 && squares == 4 && 
  ( 
    (plant == *bottomLeftPlant && plant == *topRightPlant) || (plant == *bottomRightPlant && plant == *topLeftPlant) 
    
  );

  if (p.x > 0) {
    PointEx left = { p.x - 1, p.y };
    if (isCorner(left, input, plant)) {

      std::size_t samePlants = 0;
      const auto p1 = PointEx{p.x -1,p.y - 1};
      const auto p2 = PointEx{p.x -1,p.y};
      bool first = false;
      if( p.y > 0 && plant == getPlant( p1 ) && inRegion(p1) ) {
        //if(!vertical || fromUp)
          ++samePlants;
          first = true;
      }
      if( p.y < size.width && plant == getPlant(p2) && inRegion(p2) ) {
        //if(!vertical || fromDown)
          ++samePlants;
      }
      if(samePlants == 1) {
          if(squares == 4 && vertical && isDiagonal)
          {
            if(first && fromUp && (plant == getPlant(*topLeft)) )
              neighbours.push_back(left);
            if(!first && fromDown && (plant == getPlant(*bottomLeft)))
              neighbours.push_back(left);

          }
          else if(!isDiagonal || firstNeighbours)  {
              neighbours.push_back(left);
          }

      }
    }
  }
  if (p.y > 0) {
    PointEx top = { p.x, p.y - 1 };
    if (isCorner(top, input, plant)) {
      std::size_t samePlants = 0;
      const auto p1 = PointEx{p.x,p.y - 1};
      const auto p2 = PointEx{p.x -1,p.y - 1};
      bool first = false;
      if( p.y > 0 && plant == getPlant( p1 ) && inRegion(p1) ) {
        first = true;
        //if(!horizontal || fromRight )
            ++samePlants;
      }
      if( p.x > 0 && plant == getPlant( p2 ) && inRegion(p2) ) {
       // if(!horizontal || fromLeft)
          ++samePlants;
      }
      if(samePlants == 1) {
          if(squares == 4  && horizontal && isDiagonal)
          {
            if(!first && fromLeft && (plant == getPlant(*topLeft)) )
              neighbours.push_back(top);
            if(first && fromRight && (plant == getPlant(*topRight)))
              neighbours.push_back(top);

          }
          else if(!isDiagonal || firstNeighbours)  {
              neighbours.push_back(top);
          }

      }
    }
  }

  if (p.x < size.width) {
    PointEx right = { p.x + 1, p.y };
    if (isCorner(right, input, plant)) {
      std::size_t samePlants = 0;
      const auto p1 = PointEx{p.x,p.y};
      const auto p2 = PointEx{p.x,p.y - 1};
      bool first = false; 
      if( p.y < size.height && plant == getPlant(p1) && inRegion(p1)) {
        first = true;
        ++samePlants;
      }
      if( p.y > 0  && plant == getPlant( p2 ) && inRegion(p2) ) {
           ++samePlants;
      }
      if(samePlants == 1) {
          if(squares == 4 &&  vertical && isDiagonal)
          {
            if(first && fromDown && (plant == getPlant(*bottomRight)) )
              neighbours.push_back(right);
            if(!first && fromUp && (plant == getPlant(*topRight)))
              neighbours.push_back(right);

          }
          else if(!isDiagonal || firstNeighbours) {
              neighbours.push_back(right);
          }

      }
    }
  }
  if (p.y < size.height) {
    PointEx down = { p.x, p.y + 1 };
    if (isCorner(down, input, plant)) {
      std::size_t samePlants = 0;
      const auto p1 = PointEx{p.x,p.y};
      const auto p2 = PointEx{p.x -1,p.y}; 
      bool first = false;
      if( p.y < size.height && plant == getPlant( p1 ) && inRegion(p1) ) {
        //if(!horizontal || fromRight )
        first = true;
        ++samePlants;
      }
      if( p.x > 0  && plant == getPlant(p2) && inRegion(p2) ) {
       // if(!horizontal || fromLeft )
          ++samePlants;
      }
      
      if(samePlants == 1) {
          if(squares == 4 && horizontal && isDiagonal )
          {
            if(first && fromRight && (plant == getPlant(*bottomRight)) )
              neighbours.push_back(down);
            if(!first && fromLeft && (plant == getPlant(*bottomLeft)))
              neighbours.push_back(down);

          }
          else if(!isDiagonal || firstNeighbours) {
              neighbours.push_back(down);
          }

      }}

  }
 // neighbours.resize(2);
  //assert(neighbours.size() <= 2 && neighbours.size() > 0);
  return neighbours;
}

auto
getAllEdgeCorners(const Input& input, const std::vector<Point> &edges)
{

  std::set<Point> allEdgeCorners{};

  for(const auto &p : edges) {

  const auto plant = input[p.y][p.x];
  const auto leftTop = p;
  const auto rightTop = PointEx{ p.x + 1, p.y };
  const auto leftBottom = PointEx{ p.x, p.y + 1 };
  const auto rightBottom = PointEx{ p.x + 1, p.y + 1 };

  if (isCorner(leftTop, input, plant)) {
    allEdgeCorners.insert(leftTop);
  }
  if (isCorner(rightTop, input, plant)) {
    allEdgeCorners.insert(rightTop);
  }
  if (isCorner(leftBottom, input, plant)) {
    allEdgeCorners.insert(leftBottom);
  }
  if (isCorner(rightBottom, input, plant)) {
    allEdgeCorners.insert(rightBottom);
  }

  }
  return allEdgeCorners;
}

auto
getSides(const std::vector<Point>& edges, const Input& input)
{
  auto printPoint = [](const auto p) {
   // std::println("({}, {})",p.x,p.y);
  };

  const auto edgeCount = edges.size();
  const auto plant = input[edges[0].y][edges[0].x];
  std::size_t sides = 0;
  auto allEdgePoints = getAllEdgeCorners(input,edges);
  while(!allEdgePoints.empty()) {

   // std::println("Plant {} ",plant);
    ++sides;
    auto last2 = *allEdgePoints.begin();
    const auto neighs = getNeighbours2(last2,last2,input, plant, edges,allEdgePoints);
    auto last = neighs[0];
    const auto startPoint = last2;
    allEdgePoints.erase(last2);
    allEdgePoints.erase(last);


    printPoint(last2);
    printPoint(last);


    while (true) {
      PointEx next{};
      for (const auto n : getNeighbours2(last, last2, input, plant,edges,allEdgePoints)) {
        if (n != last2) {
          next = n;
          break;
        }
      }
      if (isTurn(last2, last, next)) {
     //   std::println("Turning");
        ++sides;
      }
      printPoint(next);
      allEdgePoints.erase(next);
      if (startPoint == next) {
        break;
      }
      last2 = last;
      last = next;
    }
  }
  return sides;
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
    if (perimeterToAdd > 0) {
      edges.push_back(point);
    }
    visitedPoints.insert(point);
  }
  return Region{ { std::begin(visitedPoints), std::end(visitedPoints) },
                 perimeter,
                 getSides(edges, input) };
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
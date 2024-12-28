#include "aoc24/day8.hpp"

#include <array>
#include <cmath>
#include <compare>
#include <cstddef>
#include <cstdint>
#include <map>
#include <numeric>
#include <set>
#include <tuple>
#include <utility>
#include <vector>

namespace aoc24::day8 {

namespace {

struct Point
{
  std::uint32_t x;
  std::uint32_t y;
  constexpr std::strong_ordering operator<=>(const Point&) const = default;
};

struct Size
{
  std::size_t height;
  std::size_t width;
};

using Points = std::set<Point>;

struct Shift
{
  std::int64_t x;
  std::int64_t y;
};

auto
getDiff(Point p1, Point p2)
{
  const std::int64_t xdiff =
    p2.x >= p1.x ? p2.x - p1.x : -static_cast<std::int64_t>(p1.x - p2.x);
  const std::int64_t ydiff =
    p2.y >= p1.y ? p2.y - p1.y : -static_cast<std::int64_t>(p1.y - p2.y);

  return std::tuple{ xdiff, ydiff };
}
auto
getDistance(Point p1, Point p2)
{
  const auto& [xDiff, yDiff] = getDiff(p1, p2);
  return std::abs(xDiff) + abs(yDiff);
}

auto
getAntiNodes(Point p1, Point p2, Size size)
{
  const auto& [xDiff, yDiff] = getDiff(p1, p2);
  const auto gcd = std::gcd(std::abs(xDiff), std::abs(yDiff));

  const auto gcdx = xDiff / gcd;
  const auto gcdy = yDiff / gcd;
  const auto width = static_cast<std::int64_t>(size.width);
  const auto height = static_cast<std::int64_t>(size.height);
  std::vector<Point> antinodesPart1{};
  std::vector<Point> antinodesPart2{};
  for (const auto dir : std::array<std::int64_t, 2>{ 1, -1 }) {
    std::int64_t x = p1.x;
    std::int64_t y = p1.y;
    for (; x < width && x >= 0 && y < height && y >= 0;
         x += dir * gcdx, y += dir * gcdy) {
      const auto p =
        Point{ static_cast<uint32_t>(x), static_cast<uint32_t>(y) };
      antinodesPart2.push_back(p);
      const auto distP1 = getDistance(p, p1);
      const auto distP2 = getDistance(p, p2);
      if (distP1 == 2 * distP2 || distP2 == 2 * distP1) {
        antinodesPart1.push_back(p);
      }
    }
  }
  return std::tuple{ antinodesPart1, antinodesPart2 };
}

}
Result
solve(const Input& input)
{
  const Size size = { input.size(), input[0].size() };

  std::map<unsigned char, Points> antennas{};

  for (std::size_t y = 0; y < size.height; ++y) {
    for (std::size_t x = 0; x < size.width; ++x) {
      const auto value = input[y][x];
      if (value != '.') {
        antennas[value].insert(
          { static_cast<std::uint32_t>(x), static_cast<std::uint32_t>(y) });
      }
    }
  }
  std::set<Point> uniqueAntinodesPart1{};
  std::set<Point> uniqueAntinodesPart2{};
  for (const auto& antenna : antennas) {
    for (const auto p1 : antenna.second) {
      for (const auto p2 : antenna.second) {
        if (p1 != p2) {
          const auto& [antinodesPart1, antinodesPart2] =
            getAntiNodes(p1, p2, size);
          uniqueAntinodesPart1.insert_range(antinodesPart1);
          uniqueAntinodesPart2.insert_range(antinodesPart2);
        }
      }
    }
  }
  return { uniqueAntinodesPart1.size(), uniqueAntinodesPart2.size() };
}

}

#include "aoc24/day8.hpp"

#include <compare>
#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <utility>

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
getAntiNodes(Point p1, Point p2, Size size)
{
  std::vector<Point> antinodes{};
  const std::int64_t x =
    p2.x >= p1.x ? p2.x - p1.x : -static_cast<std::int64_t>(p1.x - p2.x);
  const std::int64_t y =
    p2.y >= p1.y ? p2.y - p1.y : -static_cast<std::int64_t>(p1.y - p2.y);

  const Shift shift = { x, y };

  auto shiftPoint = [&size, &shift](Point p, bool add) -> std::optional<Point> {
    const Shift newShift = add ? shift : Shift{ -shift.x, -shift.y };
    const Shift shiftedPoint = { p.x + newShift.x, p.y + newShift.y };
    if (shiftedPoint.x >= 0 && shiftedPoint.y >= 0 &&
        std::cmp_less(shiftedPoint.x, size.width) &&
        std::cmp_less(shiftedPoint.y, size.height)) {
      return Point{ static_cast<uint32_t>(shiftedPoint.x),
                    static_cast<uint32_t>(shiftedPoint.y) };
    }
    return std::nullopt;
  };

  const auto antinode1 = shiftPoint(p2, true);
  const auto antinode2 = shiftPoint(p1, false);

  if (antinode1) {
    antinodes.push_back(*antinode1);
  }
  if (antinode2) {
    antinodes.push_back(*antinode2);
  }
  return antinodes;
}

auto
getAntiNodes2(Point p1, Point p2, Size size)
{
  std::vector<Point> antinodes{};

  const std::int64_t x =
    p2.x >= p1.x ? p2.x - p1.x : -static_cast<std::int64_t>(p1.x - p2.x);
  const std::int64_t y =
    p2.y >= p1.y ? p2.y - p1.y : -static_cast<std::int64_t>(p1.y - p2.y);
  const std::int64_t a = y / x;
  const std::int64_t b = p1.y - (a * p1.x);

  const auto width = static_cast<std::int64_t>(size.width);
  const auto height = static_cast<std::int64_t>(size.height);

  auto line = [&](std::int64_t x) { return (a * x) + b; };

  for (std::int64_t x2 = 0; x2 < width; ++x2) {
    const auto y2 = line(x2);
    if (y2 >= 0 && y2 < height) {
      antinodes.push_back(
        { static_cast<uint32_t>(x2), static_cast<uint32_t>(y2) });
    }
  }
  return antinodes;
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
  std::set<Point> antinodes{};
  std::set<Point> antinodes2{};
  for (const auto& antenna : antennas) {
    for (const auto p1 : antenna.second) {
      for (const auto p2 : antenna.second) {
        if (p1 != p2) {
          antinodes.insert_range(getAntiNodes(p1, p2, size));
          antinodes2.insert_range(getAntiNodes2(p1, p2, size));
        }
      }
    }
  }
  const std::size_t part1 = antinodes.size();
  const std::size_t part2 = antinodes2.size();
  return { part1, part2 };
}

}

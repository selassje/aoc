#include "day17.hpp"

#include <cstddef>
#include <variant>

namespace aoc22::day17 {

struct Point
{
  std::size_t x;
  std::size_t y;
};

struct Line
{
  Point start;
  Point end;
  constexpr bool isVertical() const noexcept { return start.x == end.x; }
  bool isOverlapping(const Line& line) const noexcept
  {
    static constexpr auto inRange = [](const auto &min, const auto& val,  const auto &max) {
      return min <= val && val <= max;
    };

    if (isVertical() == line.isVertical()) {
      if (isVertical()) {
         return inRange(start.y, line.start.y, end.y) ||  inRange(start.y, line.end.y, end.y);
      } 
      return inRange(start.x, line.start.x, end.x) ||  inRange(start.x, line.end.x, end.x);
    } else {
        if (isVertical()) {
          return inRange(line.start.x, start.x, line.end.x) && inRange(start.y, line.start.y, end.y);
        }
        return inRange(line.start.y, start.y, line.end.y) && inRange(start.x, line.start.x, end.x);
    }
  }
};

using Lines = std::vector<Line>;

struct RockCommon
{
  Point bottomLeft;
};

struct HorizontalLine : RockCommon
{};
struct Cross : RockCommon
{};
struct ReversedL : RockCommon
{};
struct VerticalLine : RockCommon
{};
struct Square : RockCommon
{};

  using Rock =
    std::variant<HorizontalLine, Cross, ReversedL, VerticalLine, Square>;

auto getNextRock =
  [i = std::size_t{ 0 }](const Point& leftBottomEdge) mutable -> Rock {
  switch (i) {
    case 0:
      return HorizontalLine{ leftBottomEdge };
    case 1:
      return Cross{ leftBottomEdge };
    case 2:
      return ReversedL{ leftBottomEdge };
    case 3:
      return VerticalLine{ leftBottomEdge };
    case 4:
      return Square{ leftBottomEdge };
    default:
      std::abort();
  }
  i = (i + 1) % std::variant_size_v<Rock>;
};

Result
solve(const Input& input)
{
  static constexpr std::size_t chamberWidth = 7;
  static constexpr std::size_t targetStoppedRocksCount = 2022;
  const std::size_t jetCount = input.size();

  std::size_t heighestRock = 0;
  std::vector<Rock> stoppedRocks{};
  return { heighestRock, heighestRock };
  static constexpr std::size_t leftEdge = 2;
  while (stoppedRocks.size() < targetStoppedRocksCount) {
    auto rock = getNextRock({ leftEdge, heighestRock + 4 });
    std::size_t i = 0;
    while (true) {
      const auto jet = input[i];
      i = (i + 1) % jetCount;
    }
    stoppedRocks.emplace_back(rock);
  }

  return { heighestRock, heighestRock };
}

}
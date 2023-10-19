#include "day15.hpp"

#include <algorithm>
#include <optional>

namespace aoc22::day15 {

[[nodiscard]] constexpr std::size_t
getDistance(const Point& p1, const Point& p2) noexcept
{
  return static_cast<std::size_t>(std::max(p1.x, p2.x) - std::min(p1.x, p2.x) +
                                  std::max(p1.y, p2.y) - std::min(p1.y, p2.y));
}

struct Line
{
  Point start;
  Point end;
};

bool
tryMergeLines(Line& target, const Line& source)
{
  if ((source.start.x <= target.start.x && target.start.x <= source.end.x) ||
      (source.start.x <= target.end.x && target.end.x <= source.end.x)) {
    target.start.x = std::min(target.start.x, source.start.x);
    target.end.x = std::max(target.end.x, source.end.x);
    return true;
  }
  return false;
}

std::vector<Line>
getNonBeaconRanges(const Input& input, const std::int32_t y)
{
  std::vector<Line> lines{};
  for (const auto& [sensor, beacon] : input) {
    const auto radius = getDistance(sensor, beacon);
    const auto distanceToY =
      static_cast<std::size_t>(std::max(sensor.y, y) - std::min(sensor.y, y));
    if (radius >= distanceToY) {
      const auto radiusAtY = radius - distanceToY;
      Point start = { sensor.x - static_cast<std::int32_t>(radiusAtY), y };
      Point end = { sensor.x + static_cast<std::int32_t>(radiusAtY), y };

      if (start == beacon) {
        start.x += 1;
      }
      if (end == beacon) {
        end.x -= 1;
      }
      lines.emplace_back(start, end);
    }
  }

  bool onlyDisjointeLeft = false;
outer:
  while (!onlyDisjointeLeft) {
    for (auto itLine1 = lines.begin(); itLine1 != lines.end(); ++itLine1) {
      for (auto itLine2 = lines.begin();
           itLine2 != lines.end() && itLine1 != itLine2;
           ++itLine2) {
        if (tryMergeLines(*itLine1, *itLine2)) {
          lines.erase(itLine2);
          goto outer;
        } else if (tryMergeLines(*itLine2, *itLine1)) {
          lines.erase(itLine1);
          goto outer;
        }
      }
    }
    onlyDisjointeLeft = true;
  }

  std::ranges::sort(lines, [](const auto& line1, const auto& line2) {
    return line1.start.x < line2.start.x;
  });
  return lines;
}

template<typename std::int32_t Y, typename std::int32_t B>
Result
solve(const Input& input)
{
  std::optional<std::size_t> part1 = std::nullopt;
  for (std::int32_t y = 0; y < B && !part1; ++y) {
    auto nonBeaconRanges = getNonBeaconRanges(input, y);
    if (y == Y) {
      part1 = 0;
      for (const auto& line : nonBeaconRanges) {
        *part1 += static_cast<std::size_t>(line.end.x - line.start.x + 1);
      }
    }
  }
  return { *part1, *part1 };
}

Result
solve(const Input& input)
{
  return solve<2000000, 4000000>(input);
}

#ifdef ENABLE_TESTS
template Result
solve<10, 20>(const Input&);
#endif
}
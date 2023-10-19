#include "day15.hpp"

#include <algorithm>
#include <optional>
#include <utility>
#include <cassert>
#include <map>
#include <set>

using BeaconCountAt = std::unordered_map<std::int32_t,std::size_t>;

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
       // start.x += 1;
      }
      if (end == beacon) {
       // end.x -= 1;
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
  //

  std::set<Point> uniqueBeacons;
  BeaconCountAt beaconCountAt{};
  for (const auto& [_, beacon] : input) {
    if (!uniqueBeacons.contains(beacon)) {
      ++beaconCountAt[beacon.y];
      uniqueBeacons.insert(beacon);
    }
  }

  std::optional<std::size_t> part1 = std::nullopt;
  std::optional<std::size_t> part2 = std::nullopt;
  for (std::int32_t y = 0; y < B && !(part1 && part2); ++y) {
    auto nonBeaconRanges = getNonBeaconRanges(input, y);
    if (y == Y) {
      part1 = 0;
      for (const auto& line : nonBeaconRanges) {
        *part1 += static_cast<std::size_t>(line.end.x - line.start.x + 1) - beaconCountAt[y];
      }
    }
    std::vector<Line> potentialBeaconRanges{};
    if (nonBeaconRanges.empty()) {
      Line range = { { 0, y }, { B, y } };
      potentialBeaconRanges.emplace_back(range);
    } else {
      if (nonBeaconRanges[0].start.x <= 0) {
        nonBeaconRanges[0].start.x = 0;
      } else {
        Line range = { { 0, y }, { nonBeaconRanges[0].start.x - 1, y } };
        potentialBeaconRanges.emplace_back(range);
      }
      auto &b = nonBeaconRanges.back();
      assert(b.start.x <= b.end.x); 
      if (nonBeaconRanges.back().end.x > B) {
        nonBeaconRanges.back().end.x = B;
      } else {
        Line range = { { nonBeaconRanges.back().end.x + 1, y }, { B, y } };
        potentialBeaconRanges.emplace_back(range);
      }
      if (nonBeaconRanges.size() > 1) {
        for (std::size_t i = 1; i < nonBeaconRanges.size(); ++i) {
          const auto space =
            nonBeaconRanges[i].start.x - nonBeaconRanges[i - 1].end.x;
          if (space > 1) {
            Line range = { { nonBeaconRanges[i - 1].end.x + 1, y },
                           { nonBeaconRanges[i].start.x - 1, y } };
            potentialBeaconRanges.emplace_back(range);
          }
        }
      }

      for (const auto& range : potentialBeaconRanges) {
          if ( range.start.x == range.end.x) {
              static constexpr std::size_t freqMultiplier = 4000000;
              part2 = static_cast<std::size_t>(y) + static_cast<std::size_t>(range.start.x)*freqMultiplier;
              break;
          }
      }
    }
  }
  return { *part1, *part2 };
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
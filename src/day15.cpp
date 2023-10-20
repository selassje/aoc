#include "day15.hpp"

#include <algorithm>
#include <cassert>
#include <map>
#include <optional>
#include <set>
#include <utility>

using BeaconCountAt = std::unordered_map<std::int32_t, std::size_t>;

namespace aoc22::day15 {

[[nodiscard]] constexpr std::size_t
getDistance(const Point& p1, const Point& p2) noexcept
{
  return static_cast<std::size_t>(std::max(p1.x, p2.x) - std::min(p1.x, p2.x) +
                                  std::max(p1.y, p2.y) - std::min(p1.y, p2.y));
}

struct Range
{
  std::int32_t start;
  std::int32_t end;

  auto operator<=>(const Range&) const noexcept = default;
};

bool
tryMergeRanges(Range& target, const Range& source)
{
  const bool areOverLapping =
    (source.start <= target.start && target.start <= source.end) ||
    (source.start <= target.end && target.end <= source.end);

  const bool areAdjacent = source.start - 1 == target.end;

  if (areOverLapping || areAdjacent) {
    target.start = std::min(target.start, source.start);
    target.end = std::max(target.end, source.end);
    return true;
  }
  return false;
}

std::vector<Range>
getNonBeaconRanges(const Input& input, const std::int32_t y)
{
  std::vector<Range> ranges{};
  for (const auto& [sensor, beacon] : input) {
    const auto radius = getDistance(sensor, beacon);
    const auto distanceToY =
      static_cast<std::size_t>(std::max(sensor.y, y) - std::min(sensor.y, y));
    if (radius >= distanceToY) {
      const auto radiusAtY = radius - distanceToY;
      std::int32_t start = sensor.x - static_cast<std::int32_t>(radiusAtY);
      std::int32_t end = sensor.x + static_cast<std::int32_t>(radiusAtY);
      ranges.emplace_back(start, end);
    }
  }
  bool onlyDisjointeLeft = false;
outer:
  while (!onlyDisjointeLeft) {
    for (auto itRange1 = ranges.begin(); itRange1 != ranges.end(); ++itRange1) {
      for (auto itRange2 = ranges.begin(); itRange2 != ranges.end();
           ++itRange2) {
        if (itRange1 != itRange2 && tryMergeRanges(*itRange1, *itRange2)) {
          ranges.erase(itRange2);
          goto outer;
        }
      }
    }
    onlyDisjointeLeft = true;
  }

  std::ranges::sort(ranges);
  return ranges;
}

template<typename std::int32_t Y, typename std::int32_t B>
Result
solve(const Input& input)
{
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
      for (const auto& range : nonBeaconRanges) {
        *part1 += static_cast<std::size_t>(range.end - range.start + 1) -
                  beaconCountAt[y];
      }
    }
    std::vector<Range> potentialBeaconRanges{};
    if (nonBeaconRanges.empty()) {
      Range range = { 0, B };
      potentialBeaconRanges.emplace_back(range);
    } else {
      if (nonBeaconRanges[0].start <= 0) {
        nonBeaconRanges[0].start = 0;
      } else {
        Range range = { 0, nonBeaconRanges[0].start - 1 };
        potentialBeaconRanges.emplace_back(range);
      }
      if (nonBeaconRanges.back().end > B) {
        nonBeaconRanges.back().end = B;
      } else {
        Range range = { nonBeaconRanges.back().end + 1, B };
        potentialBeaconRanges.emplace_back(range);
      }
      if (nonBeaconRanges.size() > 1) {
        for (std::size_t i = 1; i < nonBeaconRanges.size(); ++i) {
          const auto space =
            nonBeaconRanges[i].start - nonBeaconRanges[i - 1].end;
          if (space == 2) {
            Range range = { nonBeaconRanges[i - 1].end + 1,
                            nonBeaconRanges[i].start - 1 };
            potentialBeaconRanges.emplace_back(range);
          }
        }
      }

      for (const auto& range : potentialBeaconRanges) {
        if (range.start == range.end) {
          static constexpr std::size_t freqMultiplier = 4000000;
          part2 = static_cast<std::size_t>(y) +
                  static_cast<std::size_t>(range.start) * freqMultiplier;
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
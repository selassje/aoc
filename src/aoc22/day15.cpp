#include "aoc22/day15.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <set>
#include <unordered_map>
#include <vector>

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

  constexpr auto operator<=>(const Range&) const noexcept = default;

  [[nodiscard]] constexpr std::size_t length() const noexcept
  {
    return static_cast<std::size_t>(end) - static_cast<std::size_t>(start) + 1;
  }
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
      const std::int32_t start =
        sensor.x - static_cast<std::int32_t>(radiusAtY);
      const std::int32_t end = sensor.x + static_cast<std::int32_t>(radiusAtY);
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
          goto outer; // NOLINT
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
  const auto beaconCountAtY = [&input]() {
    std::set<Point> uniqueBeacons;
    std::unordered_map<std::int32_t, std::size_t> beaconCountAtY_{}; // NOLINT
    for (const auto& [_, beacon] : input) {
      if (!uniqueBeacons.contains(beacon)) {
        ++beaconCountAtY_[beacon.y];
        uniqueBeacons.insert(beacon);
      }
    }
    return beaconCountAtY_;
  }();

  std::size_t part1 = 0;
  std::size_t part2 = 0;
  bool part1Found = false;
  bool part2Found = false;
  for (std::int32_t y = 0; y < B && !(part1Found && part2Found); ++y) {
    const auto nonBeaconRanges = getNonBeaconRanges(input, y);
    for (std::size_t i = 0; i < nonBeaconRanges.size(); ++i) {
      if (y == Y) {
        part1 += nonBeaconRanges[i].length() - beaconCountAtY.at(y);
        part1Found = true;
      }
      if (i > 0 && nonBeaconRanges[i].start - nonBeaconRanges[i - 1].end == 2) {
        static constexpr std::size_t freqMultiplier = 4000000;
        part2 = static_cast<std::size_t>(y) +
                static_cast<std::size_t>(nonBeaconRanges[i - 1].end + 1) *
                  freqMultiplier;
        part2Found = true;
      }
    }
  }
  return { part1, part2 };
}

Result
solve(const Input& input)
{
  static constexpr std::int32_t row = 2000000;
  static constexpr std::int32_t limit = 4000000;
  return solve<row, limit>(input);
}

#ifdef ENABLE_TESTS
template Result
solve<10, 20>(const Input&);
#endif
}
#include "day15.hpp"

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

template<typename std::int32_t Y>
Result
solve(const Input& input)
{
  std::size_t part1 = 0;
  for (const auto& [sensor, beacon] : input) {
    const auto radius = getDistance(sensor, beacon);
    const auto distanceToY = static_cast<std::size_t>(std::max(sensor.y, Y) - std::min(sensor.y, Y));
    if (radius >= distanceToY) {
      part1 += 2 * (radius - distanceToY) + 1  - (beacon.y == Y ? 1 : 0);
    }
  }
  return { part1, part1 };
}

Result
solve(const Input& input)
{
  return solve<2000000>(input);
}

#ifdef ENABLE_TESTS
template Result
solve<10>(const Input&);
#endif
}

#include "aoc25/day1.hpp"
#include <cstdint>

namespace {

struct RotationResult
{
  std::uint64_t position;
  std::uint64_t timesOPassed;
};

RotationResult
rotate(std::uint64_t position, const aoc25::day1::Rotation& rotation)
{
  constexpr auto maxCount = 100;
  std::uint64_t times0Passed = 0;

  if (rotation.direction == aoc25::day1::Direction::Right) {
    const auto rawShifted = position + rotation.count;
    position = rawShifted % maxCount;
    times0Passed = rawShifted / maxCount;
  } else {
    if (position >= rotation.count) {
      position -= rotation.count;
      if (position == 0) {
        times0Passed = 1;
      }
    } else {
      if (position == 0) {
         times0Passed = (rotation.count / maxCount);
      }
      else {
         times0Passed = ( (rotation.count - position) / maxCount) + 1;
      }
      position = (maxCount - ((rotation.count - position) % maxCount)) % maxCount;
    }
  }
  return { position, times0Passed };
}

}

namespace aoc25::day1 {
Result
solve(const Input& input)
{
  std::uint64_t position = 50; // NOLINT
  std::uint64_t part1 = 0;
  std::uint64_t part2 = 0;

  for (const auto& rotation : input) {
    const auto rotationResult = rotate(position, rotation);
    position = rotationResult.position;
    if (position == 0) {
      ++part1;
    }
    part2 += rotationResult.timesOPassed;
  }
  return { part1, part2 };
}
}

#include "aoc25/day2.hpp"
#include <cstdint>
#include <string>

namespace {

using aoc25::day2::Range;

bool
isInvalid(std::uint64_t id)
{
  const auto idStr = std::to_string(id);
  const auto size = idStr.size();
  if (size % 2 == 1) {
    return false;
  }
  for (std::size_t i = 0; i < size / 2; ++i) {
    if (idStr[i] != idStr[i + (size / 2)]) {
      return false;
    }
  }
  return true;
}

bool
isInvalid2(std::uint64_t id)
{
  const auto idStr = std::to_string(id);
  const auto size = idStr.size();

  for (std::size_t seqSize = 1; seqSize <= size / 2; ++seqSize) {
    if (size % seqSize != 0) {
      continue;
    }
    const auto seqCount = size / seqSize;
    bool seqExists = true;
    for (std::size_t i = 0; i < seqSize; ++i) {
      const auto first = idStr[i];
      for (std::size_t j = 0; j < seqCount; ++j) {
        if (idStr[i + (j*seqSize)] != first) {
          seqExists = false;
          break;
        }
      }
    }
    if (seqExists) {
      return true;
    }
  }
  return false;
}

std::uint64_t
invalidSum(const Range& range)
{
  std::uint64_t sum = 0;
  for (auto i = range.from; i <= range.to; ++i) {
    if (isInvalid(i)) {
      sum += i;
    }
  }
  return sum;
}

std::uint64_t
invalidSum2(const Range& range)
{
  std::uint64_t sum = 0;
  for (auto i = range.from; i <= range.to; ++i) {
    if (isInvalid2(i)) {
      sum += i;
    }
  }
  return sum;
}

}

namespace aoc25::day2 {
Result
solve(const Input& input)
{
  std::uint64_t part1 = 0;
  std::uint64_t part2 = 0;

  for (const auto& range : input) {
    part1 += invalidSum(range);
    part2 += invalidSum2(range);
  }
  return { part1, part2 };
}
}
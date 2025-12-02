
#include "aoc25/day2.hpp"
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace {

using aoc25::day2::Range;
bool
isInvalid(std::string_view idStr, std::size_t seqSize)
{
  if (seqSize == 0) {
    return false;
  }
  const auto size = idStr.size();
  if (size % seqSize != 0) {
    return false;
  }
  const auto seqCount = size / seqSize;
  for (std::size_t i = 0; i < seqSize; ++i) {
    const auto first = idStr[i];
    for (std::size_t j = 0; j < seqCount; ++j) {
      if (idStr[i + (j * seqSize)] != first) {
        return false;
      }
    }
  }
  return true;
}

auto
invalidSums(const Range& range)
{
  struct
  {
    std::uint64_t twoRepeatsSum{};
    std::uint64_t anyRepeatsSum{};
  } sums;
  for (auto id = range.from; id <= range.to; ++id) {
    const auto idStr = std::to_string(id);
    const auto size = idStr.size();
    if (size % 2 == 0 && isInvalid(idStr, size / 2)) {
      sums.twoRepeatsSum += id;
    }
    for (std::size_t i = 1; i <= size / 2; ++i) {
      if (isInvalid(idStr, i)) {
        sums.anyRepeatsSum += id;
        break;
      }
    }
  }
  return sums;
}

}

namespace aoc25::day2 {
Result
solve(const Input& input)
{
  std::uint64_t part1 = 0;
  std::uint64_t part2 = 0;
  for (const auto& range : input) {
    const auto& [twoRepeatsSum, anyRepeatsSum] = invalidSums(range);
    part1 += twoRepeatsSum;
    part2 += anyRepeatsSum;
  }
  return { part1, part2 };
}
}
#include "aoc25/day3.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>

namespace {
using aoc25::day3::Bank;
std::uint64_t
getMaxJoltage(const Bank& bank)
{
  auto sortedBank = bank;
  std::ranges::sort((sortedBank));
  return static_cast<std::uint64_t>(sortedBank[0] * sortedBank[1]);
}
}

namespace aoc25::day3 {
Result
solve(const Input& input)
{
  std::uint64_t part1 = 0;
  std::uint64_t part2 = 0;
  for (const auto& bank : input) {
    part1 += getMaxJoltage(bank);
  }
  part2 = part1;
  return { part1, part2 };
}
}
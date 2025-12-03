#include "aoc25/day3.hpp"
#include <algorithm>
#include <atomic>
#include <cstddef>
#include <cstdint>

namespace {
using aoc25::day3::Bank;
std::uint64_t
getMaxJoltage(const Bank& bank)
{
  constexpr static auto base = static_cast<std::uint64_t>(10);
  constexpr static auto zero = static_cast<unsigned char>('0');
  std::uint64_t max = 0;
  for(auto itFirst = bank.begin(); itFirst != bank.end() - 1; ++itFirst) {
      const auto second = static_cast<std::uint64_t>(*std::max_element(itFirst + 1, bank.end()) - zero);
      const auto first = static_cast<std::uint64_t>(*itFirst - zero);
      max = std::max({max, first*base + second}); //NOLINT
  }
  return max;
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
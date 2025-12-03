#include "aoc25/day3.hpp"
#include <algorithm>
#include <atomic>
#include <cmath>
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
  for (auto itFirst = bank.begin(); itFirst != bank.end() - 1; ++itFirst) {
    const auto second = static_cast<std::uint64_t>(
      *std::max_element(itFirst + 1, bank.end()) - zero);
    const auto first = static_cast<std::uint64_t>(*itFirst - zero);
    max = std::max({ max, first * base + second }); // NOLINT
  }
  return max;
}

std::uint64_t
getMaxJoltage2(const Bank& bank, std::size_t batteriesCount)
{
  constexpr static auto base = static_cast<std::uint64_t>(10);
  constexpr static auto zero = static_cast<unsigned char>('0');

  const auto size = bank.size();

  if (batteriesCount > size) {
    return 0;
  }
  std::uint64_t max = 0;
  auto beginIt = bank.begin();
  for (std::size_t battery = 0; battery < batteriesCount; ++battery) {
    const auto endOffset = static_cast<int>( batteriesCount - battery - 1);
    const auto endIt = bank.end() - endOffset;
    const auto maxBatteryIt = std::max_element(beginIt, endIt);
    const std::uint64_t batteryJoltage = *maxBatteryIt - zero;
    beginIt = maxBatteryIt + 1;
    const auto order = static_cast<std::uint64_t>(std::pow(10,batteriesCount - battery - 1));
    max += order*batteryJoltage; 
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
    part1 += getMaxJoltage2(bank,2);
    part2 += getMaxJoltage2(bank,12);
  }
  return { part1, part2 };
}
}
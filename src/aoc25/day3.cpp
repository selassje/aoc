#include "aoc25/day3.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
namespace {
using aoc25::day3::Bank;

template<std::size_t count>
constexpr auto
getOrders()
{
  constexpr static auto base = 10;
  std::array<std::uint64_t, count> orders{};
  orders[count - 1] = 1;
  for (std::size_t i = 1; i < count; ++i) {
    orders[count - i - 1] = orders[count - i] * base;
  }
  return orders;
}

template<std::size_t BATTERIES_COUNT>
std::uint64_t
getMaxJoltage(const Bank& bank)
{
  constexpr static auto zero = static_cast<unsigned char>('0');
  constexpr static auto orders = getOrders<BATTERIES_COUNT>();
  const auto size = bank.size();

  if (BATTERIES_COUNT > size) {
    return 0;
  }
  std::uint64_t max = 0;
  auto beginIt = bank.begin();
  for (std::size_t battery = 0; battery < BATTERIES_COUNT; ++battery) {
    const auto endOffset = static_cast<int>(BATTERIES_COUNT - battery - 1);
    const auto endIt = bank.end() - endOffset;
    const auto maxBatteryIt = std::max_element(beginIt, endIt);
    const std::uint64_t batteryJoltage = *maxBatteryIt - zero;
    beginIt = maxBatteryIt + 1;
    max += orders[battery] * batteryJoltage;
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
    part1 += getMaxJoltage<2>(bank);
    part2 += getMaxJoltage<12>(bank); // NOLINT
  }
  return { part1, part2 };
}
}
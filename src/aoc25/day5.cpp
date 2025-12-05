
#include "aoc25/day5.hpp"
#include <algorithm>

static bool
isInRange(std::uint64_t value, const aoc25::day5::Range& range)
{
  return value >= range.from && value <= range.to;
}

static bool
isFresh(std::uint64_t id, const aoc25::day5::Ranges& ranges)
{
  return std::ranges::any_of(
    ranges, [&](const auto& range) { return isInRange(id, range); });
}

namespace aoc25::day5 {
Result
solve(const Input& input)
{
  const auto  part1  = static_cast<std::uint64_t>(std::count_if(
    input.availableIndgredientIds.begin(),
    input.availableIndgredientIds.end(),
    [&](const auto id) { return isFresh(id, input.freeshIgredients); }));
  return { part1, part1 };
}
}
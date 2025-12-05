
#include "aoc25/day5.hpp"
#include <algorithm>
#include <optional>
#include <vector>

using aoc25::day5::Range;
using aoc25::day5::Ranges;

static bool
isInRange(std::uint64_t value, const Range& range)
{
  return value >= range.from && value <= range.to;
}

static bool
isFresh(std::uint64_t id, const Ranges& ranges)
{
  return std::ranges::any_of(
    ranges, [&](const auto& range) { return isInRange(id, range); });
}

static std::optional<Range>
mergeRanges(const Range& first, const Range& second)
{
  if (first.to + 1 < second.from || second.to + 1 < first.from) {
    return std::nullopt;
  }
  return Range{ std::min(first.from, second.from),
                std::max(first.to, second.to) };
}

static Ranges
getUniqueRanges(const Ranges& ranges)
{
  if (ranges.empty()) {
    return {};
  }
  Ranges sortedRanges = ranges;
  std::ranges::sort(
    sortedRanges, [](const auto& a, const auto& b) { return a.from < b.from; });
  Ranges uniqueRanges;
  uniqueRanges.push_back(sortedRanges[0]);
  for (std::size_t i = 1; i < sortedRanges.size(); ++i) {
    const auto& currentRange = sortedRanges[i];
    auto& lastUniqueRange = uniqueRanges.back();
    if (const auto mergedRange = mergeRanges(lastUniqueRange, currentRange);
        mergedRange.has_value()) {
      lastUniqueRange = *mergedRange;
    } else {
      uniqueRanges.push_back(currentRange);
    }
  }
  return uniqueRanges;
}

namespace aoc25::day5 {
Result
solve(const Input& input)
{
  const auto part1 = static_cast<std::uint64_t>(std::count_if(
    input.availableIngredientIds.begin(),
    input.availableIngredientIds.end(),
    [&](const auto id) { return isFresh(id, input.freeshIngredients); }));

  const auto uniqueRanges = getUniqueRanges(input.freeshIngredients);

  std::uint64_t part2 = 0;
  for (const auto& range : uniqueRanges) {
    part2 += (range.to - range.from + 1);
  }

  return { part1, part2 };
}
}
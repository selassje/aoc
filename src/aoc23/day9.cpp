#include "aoc23/day9.hpp"

#include <algorithm>

namespace aoc23::day9 {

auto
extrapolate(const Values& originalValues)
{
  using Sequences = Input;
  Sequences sequences{ originalValues };
  auto isNonZero = [](const Values& values) {
    return std::ranges::find_if_not(
             values, [](const auto& v) { return v == 0; }) == values.end();
  };
  std::size_t size = originalValues.size();
  while (!isNonZero(sequences.back())) {
    sequences.emplace_back(size - 1, 0);
    for (std::size_t i = 0; i < size - 1; ++i) {
      const auto j = sequences.size() - 1;
      sequences[j][i] = sequences[j - 1][i + 1] - sequences[j - 1][i];
    }
    --size;
  }

  sequences.back().push_back(0);
  auto i = sequences.size() - 2;
  while (true) {
    sequences[i].push_back(sequences[i].back() + sequences[i + 1].back());
    if (i-- == 0) {
      break;
    }
  }

  return sequences[0].back();
}

Result
solve(const Input& input)
{
  std::int64_t part1 = 0;
  for (const auto& values : input) {
    part1 += extrapolate(values);
  }

  return { part1, part1 };
}

}
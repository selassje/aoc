#include "aoc23/day9.hpp"

#include <algorithm>
#include <utility>

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
  auto forwardSequences = sequences;
  auto backwardSequences = sequences;

  forwardSequences.back().push_back(0);
  backwardSequences.back().insert(backwardSequences.back().begin(), 0);
  auto i = forwardSequences.size() - 2;
  while (true) {
    forwardSequences[i].push_back(forwardSequences[i].back() +
                                  forwardSequences[i + 1].back());
    backwardSequences[i].insert(backwardSequences[i].begin(),
                                backwardSequences[i][0] -
                                  backwardSequences[i + 1][0]);
    if (i-- == 0) {
      break;
    }
  }

  return std::make_pair(forwardSequences[0].back(), backwardSequences[0][0]);
}

Result
solve(const Input& input)
{
  std::int64_t part1 = 0;
  std::int64_t part2 = 0;
  for (const auto& values : input) {
    const auto [forward, backward] = extrapolate(values);
    part1 += forward;
    part2 += backward;
  }

  return { part1, part2 };
}

}
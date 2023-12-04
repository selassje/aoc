#ifndef AOC23_DAY4_HPP
#define AOC23_DAY4_HPP

#include <cstddef>
#include <vector>

namespace aoc23::day4 {

struct Card {
  std::vector<std::size_t> winningNumbers;
  std::vector<std::size_t> actualNumbers;
};

using Input = std::vector<Card>;

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

Result
solve(const Input& input);

}
#endif
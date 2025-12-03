
#ifndef AOC25_DAY3_HPP
#define AOC25_DAY3_HPP

#include <cstdint>
#include <vector>

namespace aoc25::day3 {

using Bank = std::vector<std::uint8_t>;
using Input = std::vector<Bank>;

struct Result
{
  std::uint64_t part1;
  std::uint64_t part2;
};

Result
solve(const Input& input);

}

#endif
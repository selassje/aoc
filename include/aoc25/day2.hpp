
#ifndef AOC25_DAY2_HPP
#define AOC25_DAY2_HPP

#include <cstdint>
#include <vector>

namespace aoc25::day2 {

struct Range
{
  std::uint64_t from;
  std::uint64_t to;
};

using Input = std::vector<Range>;

struct Result
{
  std::uint64_t part1;
  std::uint64_t part2;
};

Result
solve(const Input& input);

}

#endif
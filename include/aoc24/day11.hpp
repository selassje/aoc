#ifndef AOC24_DAY11_HPP
#define AOC24_DAY11_HPP

#include <cstddef>
#include <vector>

namespace aoc24::day11 {

using Input = std::vector<std::size_t>;

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

Result
solve(const Input& input);

}

#endif
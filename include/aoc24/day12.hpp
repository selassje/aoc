
#ifndef AOC24_DAY12_HPP
#define AOC24_DAY12_HPP

#include <cstddef>
#include <vector>

namespace aoc24::day12 {

using Input = std::vector<std::vector<unsigned char>>;

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

Result
solve(const Input& input);

}

#endif
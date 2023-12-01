#ifndef AOC23_DAY1_HPP
#define AOC23_DAY1_HPP

#include <cstddef>
#include <string>
#include <vector>

namespace aoc23::day1 {

using Input = std::vector<std::string>;

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

Result
solve(const Input& input);

}
#endif

#ifndef AOC22_DAY17_HPP
#define AOC22_DAY17_HPP

#include <cstddef>
#include <vector>

namespace aoc22::day17 {

enum class JetDir
{
  Left,
  Right
};

using Input = std::vector<JetDir>;

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

Result
solve(const Input& input);

}
#endif
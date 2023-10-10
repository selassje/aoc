
#ifndef AOC22_DAY13_HPP
#define AOC22_DAY13_HPP

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

namespace aoc22::day13 {

using Pair = std::pair<std::string, std::string>;
using Input = std::vector<Pair>;

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

[[nodiscard]] Result
solve(const Input& input);

}

#endif
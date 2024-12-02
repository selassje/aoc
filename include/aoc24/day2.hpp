
#ifndef AOC24_DAY2_HPP
#define AOC24_DAY2_HPP

#include <cstddef>
#include <vector>

namespace aoc24::day2 {

using Input = std::vector<std::vector<std::size_t>>;

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

[[nodiscard]] Result
solve(const Input& iinput) noexcept;

}

#endif
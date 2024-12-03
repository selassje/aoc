
#ifndef AOC24_DAY3_HPP
#define AOC24_DAY3_HPP

#include <cstddef>
#include <string_view>

namespace aoc24::day3 {

using Input = std::string_view;

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

[[nodiscard]] Result
solve(Input input) noexcept;

}

#endif
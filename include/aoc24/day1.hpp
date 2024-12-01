#ifndef AOC24_DAY1_HPP
#define AOC24_DAY1_HPP

#include <cstdint>
#include <span>
#include <utility>
namespace aoc24::day1 {

using Input = std::span<std::pair<std::uint32_t, std::uint32_t>>;

struct Result
{
  std::uint32_t part1;
  std::uint32_t part2;
};

Result
solve(Input input);

}

#endif
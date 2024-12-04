
#ifndef AOC24_DAY4_HPP
#define AOC24_DAY4_HPP

#include <cstddef>
#include <cstdint>
#include <vector>

namespace aoc24::day4 {

enum class Letter : std::uint8_t
{
  X = 0,
  M = 1,
  A = 2,
  S = 3
};

using Input = std::vector<std::vector<Letter>>;

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

[[nodiscard]] Result
solve(const Input&) noexcept;

}

#endif
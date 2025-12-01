
#ifndef AOC25_DAY1_HPP
#define AOC25_DAY1_HPP

#include <cstdint>
#include <vector>


namespace aoc25::day1 {

enum class Direction : std::uint8_t {
  Left = 0,
  Right = 1
};

struct Rotation {
  Direction direction;
  std::uint64_t count;
};

using Input = std::vector<Rotation>;

struct Result
{
  std::uint64_t part1;
  std::uint64_t part2;
};

Result
solve(const Input &input);

}

#endif

#ifndef AOC24_DAY6_HPP
#define AOC24_DAY6_HPP

#include <cstddef>
#include <cstdint>
#include <vector>

namespace aoc24::day6 {

enum class Tile : std::uint8_t
{
  Empty,
  Obstacle,
  Guard,
};

using Input = std::vector<std::vector<Tile>>;

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

Result
solve(const Input& input);

}

#endif

#ifndef AOC25_DAY4_HPP
#define AOC25_DAY4_HPP

#include <cstdint>
#include <vector>

namespace aoc25::day4 {

using Bank = std::vector<std::uint8_t>;

enum class Tile : std::uint8_t
{
  Empty,
  Roll,
};

using Input = std::vector<std::vector<Tile>>;

struct Result
{
  std::uint64_t part1;
  std::uint64_t part2;
};

Result
solve(const Input& input);

}

#endif
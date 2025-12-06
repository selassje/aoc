

#ifndef AOC25_DAY6_HPP
#define AOC25_DAY6_HPP

#include <cstdint>
#include <vector>

namespace aoc25::day6 {

enum class Operation : std::uint8_t
{
  Add,
  Multiply,
};
enum class Alignment : std::uint8_t
{
  Left,
  Right,
};

struct Problem
{
  std::vector<std::uint64_t> operands;
  Operation operation = Operation::Add;
  Alignment alignment = Alignment::Left;
};

using Input = std::vector<Problem>;

struct Result
{
  std::uint64_t part1;
  std::uint64_t part2;
};

Result
solve(const Input& input);

}
#endif
#ifndef AOC23_DAY8_HPP
#define AOC23_DAY8_HPP

#include <cstddef>
#include <string>
#include <vector>

namespace aoc23::day8 {

enum class Direction {
  Left,
  Right
};

using Directions = std::vector<Direction>;

struct Node {
  std::string id;
  std::string left;
  std::string right;
};

using Nodes = std::vector<Node>;


struct Input
{
  Directions directions;
  Nodes nodes;
};

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

Result
solve(const Input& input);
}

#endif
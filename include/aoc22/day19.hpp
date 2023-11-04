#ifndef AOC22_DAY19_HPP
#define AOC22_DAY19_HPP

#include <cstddef>
#include <vector>

namespace aoc22::day19 {
struct Blueprint
{
  int id;
  struct
  {
    int ore;
  } oreRobotCost;
  struct
  {
    int ore;
  } clayRobotCost;
  struct
  {
    int ore;
    int clay;
  } obsidianRobotCost;
  struct
  {
    int ore;
    int obsidian;
  } geodeRobotCost;
};

using Input = std::vector<Blueprint>;

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

Result
solve(const Input& input);

}

#endif
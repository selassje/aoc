
#ifndef AOC22_DAY16_HPP
#define AOC22_DAY16_HPP

#include <cstddef>
#include <string>
#include <vector>

namespace aoc22::day16 {

struct Valve
{
  std::string name;
  std::size_t flowRate;
  std::vector<std::string> connectedValves;
};

using Input = std::vector<Valve>;

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

Result
solve(const Input& input);

}

#endif
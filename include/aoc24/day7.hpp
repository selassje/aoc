
#ifndef AOC24_DAY7_HPP
#define AOC24_DAY7_HPP


#include <vector>
namespace aoc24::day7 {


struct Record {
  std::size_t testValue;
  std::vector<std::size_t> operands;
};

using Input = std::vector<Record>;


struct Result {
  std::size_t part1;
  std::size_t part2;
};

Result solve(const Input& input);

}

#endif
#ifndef AOC22_DAY5_HPP
#define AOC22_DAY5_HPP

#include <deque>
#include <stack>
#include <string>
#include <utility>
#include <vector>

namespace aoc22::day5 {

using Stack = std::stack<unsigned char>;
using Stacks = std::vector<Stack>;

struct Move
{
  std::size_t From;
  std::size_t To;
  std::size_t Quantity;
}; // __attribute__((aligned(4)));

using Moves = std::vector<Move>;

struct Input
{
  Stacks stacks;
  Moves moves;
}; // __attribute__((aligned(64))); // NOLINT

std::pair<std::string, std::string>
solve(const Input& input);
};
#endif
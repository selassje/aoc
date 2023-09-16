#ifndef AOC22_DAY5_HPP
#define AOC22_DAY5_HPP

#include <deque>
#include <stack>
#include <string>
#include <utility>
#include <vector>

namespace aoc22::day5 {

using Stack = std::stack<char>;
using Stacks = std::vector<Stack>;

struct alignas(32) Move
{
  std::size_t from;
  std::size_t to;
  std::size_t quantity;
};
using Moves = std::vector<Move>;

struct alignas(64) Input
{
  Stacks stacks;
  Moves moves;
};

std::pair<std::string, std::string>
solve(const Input& input);
};
#endif
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

constexpr size_t MOVE_ALIGNMENT = 32;

struct Move
{
  std::size_t From;
  std::size_t To;
  std::size_t Quantity;
}__attribute__((aligned(MOVE_ALIGNMENT)));

using Moves = std::vector<Move>;

constexpr size_t INPUT_ALIGNMENT = 64;
struct Input
{
  Stacks stacks;
  Moves moves;
}__attribute__((aligned(INPUT_ALIGNMENT)));

std::pair<std::string, std::string>
solve(const Input& input);
};
#endif
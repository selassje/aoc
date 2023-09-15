#ifndef AOC22_DAY5_HPP
#define AOC22_DAY5_HPP

#include <deque>
#include <stack>
#include <string>
#include <utility>
#include <vector>

namespace aoc22::day5 {

#ifndef WIN32
constexpr size_t MOVE_ALIGNMENT = 32;
constexpr size_t INPUT_ALIGNMENT = 64;
#endif

using Stack = std::stack<char>;
using Stacks = std::vector<Stack>;

struct alignas(MOVE_ALIGNMENT) Move
{
  std::size_t from;
  std::size_t to;
  std::size_t quantity;
};
using Moves = std::vector<Move>;

struct alignas(INPUT_ALIGNMENT) Input
{
  Stacks stacks;
  Moves moves;
};

std::pair<std::string, std::string>
solve(const Input& input);
};
#endif
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

struct Move
{
  std::size_t From;
  std::size_t To;
  std::size_t Quantity;
}
#ifndef WIN32
__attribute__((aligned(MOVE_ALIGNMENT)));
#endif
;
using Moves = std::vector<Move>;

struct Input
{
  Stacks stacks;
  Moves moves;
}
#ifndef WIN32
__attribute__((aligned(INPUT_ALIGNMENT)));
#endif
;
std::pair<std::string, std::string>
solve(const Input& input);
};
#endif
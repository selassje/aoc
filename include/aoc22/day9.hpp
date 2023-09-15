#include <cstdlib>
#include <utility>
#include <vector>

namespace aoc22::day9 {

enum class Direction
{
  Left,
  Right,
  Up,
  Down
};

struct Move {
  Direction direction;
  std::size_t count;
};

using Moves = std::vector<Move>;

std::pair<std::size_t, std::size_t>
solve(const Moves& moves);

};
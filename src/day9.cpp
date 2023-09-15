#include "day9.hpp"

#include <cstdint>
#include <unordered_set>

namespace aoc22::day9 {
struct Position
{
  std::int64_t x = 0;
  std::int64_t y = 0;
};

};

template<>
struct std::hash<aoc22::day9::Position>
{
  std::size_t operator()(const aoc22::day9::Position p) const noexcept
  {
    std::size_t h1 = std::hash<std::int64_t>{}(p.x);
    std::size_t h2 = std::hash<std::int64_t>{}(p.y);
    return h1 ^ (h2 << 1);
  }
};

namespace aoc22::day9 {

using enum Direction;

void
performMove(Position& position, Direction direction)
{
  switch (direction) {
    case Up:
      position.y += 1;
      break;
    case Down:
      position.y -= 1;
      break;
    case Left:
      position.x -= 1;
      break;
    case Right:
      position.x += 1;
      break;
  }
}

std::pair<std::size_t, std::size_t>
solve(const Moves& moves)
{
  const std::size_t resultPart2 = 0;

  Position head{};
  Position tail{};

  std::unordered_set<Position> visitedPostions{};

  for( const auto& move : moves)
  {
    for ( std::size_t i = 0 ; i < move.count ; ++i)
    {
      performMove(head, move.direction);
      

    }
  }

  const std::size_t resultPart1 = visitedPostions.size();
  return std::make_pair(resultPart1, resultPart2);
}

};

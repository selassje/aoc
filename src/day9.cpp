#include "day9.hpp"

#include <cstdint>
#include <iostream>
#include <unordered_set>

namespace aoc22::day9 {
struct Position
{
  std::int64_t x = 0;
  std::int64_t y = 0;
  bool operator<=>(const Position&) const = default;
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

enum class DirectionEx
{
  Left,
  Right,
  Up,
  Down,
  LeftUp,
  LeftDown,
  RightUp,
  RightDown
};

using enum DirectionEx;

void
performMove(Position& position, DirectionEx direction)
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
    case LeftUp:
      position.x -= 1;
      position.y += 1;
      break;
    case LeftDown:
      position.x -= 1;
      position.y -= 1;
      break;
    case RightUp:
      position.x += 1;
      position.y += 1;
      break;
    case RightDown:
      position.x += 1;
      position.y -= 1;
      break;
  }
}

bool
areTouching(const Position& p1, const Position& p2)
{
  return p1.x >= p2.x - 1 && p1.x <= p2.x + 1 && p1.y >= p2.y - 1 &&
         p1.y <= p2.y + 1;
}

std::pair<std::size_t, std::size_t>
solve(const Moves& moves)
{
  std::array<Position, 10> knots;
  std::unordered_set<Position> visitedPositions1{ {} };
  std::unordered_set<Position> visitedPositions9{ {} };

  for (const auto& move : moves) {
    for (std::size_t i = 0; i < move.count; ++i) {
      const auto directionEx = static_cast<DirectionEx>(move.direction);
      performMove(knots[0], directionEx);
      for (std::size_t k = 1; k < knots.size(); ++k) {
        const auto& head = knots[k - 1];
        auto& tail = knots[k];
        if (!areTouching(head, tail)) {
          if (head.x == tail.x || head.y == tail.y) {
            performMove(tail, directionEx);
          } else {
            const auto deltaX = tail.x < head.x ? 1 : -1;
            const auto deltaY = tail.y < head.y ? 1 : -1;
            tail.x += deltaX;
            tail.y += deltaY;
          }
         // std::cout << tail.x << " " << tail.y << std::endl;
          if (k == 1) {
            visitedPositions1.insert(tail);
          }
          if (k == 9) {
            visitedPositions9.insert(tail);
          }
        }
      }
      // std::cout << "h " <<  head.x << " " << head.y << std::endl;
    }
  }
  return std::make_pair(visitedPositions1.size(), visitedPositions9.size());
}

};

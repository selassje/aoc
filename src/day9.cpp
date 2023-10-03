#include "day9.hpp"

#include <array>
#include <compare>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <unordered_set>
#include <utility>

namespace aoc22::day9 {
struct alignas(16) Position
{
  std::int64_t x = 0;
  std::int64_t y = 0;
  std::strong_ordering operator<=>(const Position&) const = default;
};

};

template<>
struct std::hash<aoc22::day9::Position>
{
  std::size_t operator()(const aoc22::day9::Position p) const noexcept
  {
    const std::size_t h1 = std::hash<std::int64_t>{}(p.x);
    const std::size_t h2 = std::hash<std::int64_t>{}(p.y);
    return h1 ^ (h2 << 1U);
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

bool
areTouching(const Position& p1, const Position& p2)
{
  return p1.x >= p2.x - 1 && p1.x <= p2.x + 1 && p1.y >= p2.y - 1 &&
         p1.y <= p2.y + 1;
}

std::pair<std::size_t, std::size_t>
solve(const Moves& moves)
{
  static constexpr std::size_t knotsNumber = 10;
  static constexpr std::size_t tailPart1 = 1;
  static constexpr std::size_t tailPart2 = 9;

  std::array<Position, knotsNumber> knots{};
  std::unordered_set<Position> visitedPositionsPart1{ {} };
  std::unordered_set<Position> visitedPositionPart2{ {} };

  for (const auto& move : moves) {
    for (std::size_t i = 0; i < move.count; ++i) {
      performMove(knots[0], move.direction);
      for (std::size_t k = 1; k < knots.size(); ++k) {
        const auto& head = knots.at(k - 1);
        auto& tail = knots.at(k);
        if (!areTouching(head, tail)) {
          int deltaX = 0;
          int deltaY = 0;
          if (head.x == tail.x) {
            deltaY = tail.y < head.y ? 1 : -1;
          } else if (head.y == tail.y) {
            deltaX = tail.x < head.x ? 1 : -1;
          } else {
            deltaX = tail.x < head.x ? 1 : -1;
            deltaY = tail.y < head.y ? 1 : -1;
          }
          tail.x += deltaX;
          tail.y += deltaY;
          if (k == tailPart1) {
            visitedPositionsPart1.insert(tail);
          }
          if (k == tailPart2) {
            visitedPositionPart2.insert(tail);
          }
        }
      }
    }
  }
  return std::make_pair(visitedPositionsPart1.size(),
                        visitedPositionPart2.size());
}

};

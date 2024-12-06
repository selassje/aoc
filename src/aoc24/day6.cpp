#include "aoc23/day9.hpp"
#include "aoc24/day6.hpp"
#include <compare>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <set>
#include <utility>

namespace aoc24::day6 {

struct Position
{
  std::size_t x;
  std::size_t y;

  constexpr std::strong_ordering operator<=>(const Position&) const = default;

};

enum class Direction : std::uint8_t
{
  N,
  E,
  S,
  W
};

struct GuardState
{
  Position position;
  Direction direction;
};

namespace {

std::optional<GuardState>
moveGuard(const GuardState& guard, const Input& input)
{
  const auto height = input.size();
  const auto width = input[0].size();
  const auto& [x, y] = guard.position;

  auto nextPosition = [&]() -> std::optional<Position> {
    switch (guard.direction) {
      case Direction::N:
        if (y > 0) {
          return Position{ x, y - 1 };
        }
        break;
      case Direction::E:
        if (x < width - 1) {
          return Position{ x + 1, y };
        }
        break;
      case Direction::S:
        if (y < height - 1) {
          return Position{ x, y + 1 };
        }
        break;
      case Direction::W:
        if (x > 0) {
          return Position{ x - 1, y };
        }
        break;
    }
    return std::nullopt;
  }();

  if (nextPosition) {
    if (input[nextPosition->y][nextPosition->x] != Tile::Obstacle) {
      return GuardState{ *nextPosition, guard.direction };
    }
    const auto newDirection =
      static_cast<Direction>((std::to_underlying(guard.direction) + 1) % 4);
    return GuardState{ guard.position, newDirection };
  }
  return std::nullopt;
}

auto
performSimulation(const Position& start, const Input& input)
{
  std::set<Position> visitedPositions{ start };
  std::optional<GuardState> guard = GuardState{ { start }, Direction::N };
  while ((guard = moveGuard(*guard, input))) {
    visitedPositions.insert(guard->position);
  }
  return visitedPositions.size();
}

}

Result
solve(const Input& input)
{
  const auto height = input.size();
  const auto width = input[0].size();

  const auto startPosition = [&]() {
    for (std::size_t y = 0; y < height; ++y) {
      for (std::size_t x = 0; x < width; ++x) {
        if (input[y][x] == Tile::Guard) {
          return Position{ x, y };
        }
      }
    }
    std::unreachable();
  }();

  const std::size_t part1 = performSimulation(startPosition, input);
  const std::size_t part2 = part1;
  return { part1, part2 };
}

}
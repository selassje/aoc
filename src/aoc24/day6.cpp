#include "aoc24/day6.hpp"
#include <cstddef>
#include <cstdint>
#include <optional>

namespace aoc24::day6 {

struct Position {
  std::size_t x;
  std::size_t y;
};

enum class Direction : std::uint8_t {
    N,
    E,
    S,
    W
};

struct GuardState {
  Position position;
  Direction direction;
};

namespace {

  std::optional<GuardState> moveGuard(const GuardState& guard) {
      switch (guard.direction) {
          case Tile::

      }
      return std::nullopt;
  }    



}


Result
solve(const Input& input)
{
  const std::size_t part1 = input.size();
  const std::size_t part2 = input.size();
  return { part1, part2 };
}

}
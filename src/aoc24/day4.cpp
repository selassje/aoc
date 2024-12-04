#include "aoc24/day4.hpp"
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <queue>
#include <utility>

namespace {

enum class Direction : std::uint8_t
{
  W = 0,
  E = 1,
  S = 2,
  N = 3,
  NE = 4,
  NW = 5,
  SE = 6,
  SW = 7,
};

struct Position
{
  std::size_t x;
  std::size_t y;
};

using Neighbours = std::array<std::optional<Position>, 8>;
using enum Direction;

auto
getNeighbours(const Position& position, const Position& bottomRight)
{
  Neighbours neighbours{};
  const auto [x, y] = position;
  const auto [w, h] = bottomRight;
  if (x > 0) {
    neighbours[static_cast<std::size_t>(W)] = { x - 1, y };
  }
  if (x < w - 1) {
    neighbours[static_cast<std::size_t>(E)] = { x + 1, y };
  }
  if (y < h - 1) {
    neighbours[static_cast<std::size_t>(S)] = { x, y + 1 };
  }
  if (y > 0) {
    neighbours[static_cast<std::size_t>(N)] = { x, y - 1 };
  }
  if (x < w - 1 && y > 0) {
    neighbours[static_cast<std::size_t>(NE)] = { x + 1, y - 1 };
  }
  if (x > 0 && y > 0) {
    neighbours[static_cast<std::size_t>(NW)] = { x - 1, y - 1 };
  }
  if (x < w - 1 && y < h - 1) {
    neighbours[static_cast<std::size_t>(SE)] = { x + 1, y + 1 };
  }
  if (x > 0 && y < h - 1) {
    neighbours[static_cast<std::size_t>(SW)] = { x - 1, y + 1 };
  }
  return neighbours;
}

auto
getNextLetter(aoc24::day4::Letter letter)
{
  return static_cast<aoc24::day4::Letter>(std::to_underlying(letter) + 1);
}

}

namespace aoc24::day4 {

Result
solve(const Input& input) noexcept
{
  const auto h = input.size();
  const auto w = input[0].size();
  std::size_t part1 = 0;

  auto getNeighbours = [&w, &h](const Position& position) {
    return ::getNeighbours(position, Position{ w, h });
  };

  for (std::size_t y = 0; y < h; ++y) {
    for (std::size_t x = 0; x < w; ++x) {
      if ( input[y][x] != Letter::X) {
        continue;
      }
      std::queue<Position> positionsToCheck{};
      positionsToCheck.push({ x, y });
      while (!positionsToCheck.empty()) {
        const auto currentPosition = positionsToCheck.front();
        auto  currentLetter = input[currentPosition.y][currentPosition.x];
        const auto nextLetter = getNextLetter(currentLetter);
        if (currentLetter == input[currentPosition.y][currentPosition.x]) {
          if (currentLetter == Letter::S) {
            ++part1;
          } else {
            for (const auto& neighbour : getNeighbours(currentPosition)) {
              if (neighbour && input[neighbour->y][neighbour->x] == nextLetter ) {
                positionsToCheck.push(*neighbour);
              }
            }
          //  currentLetter = nextLetter;
          }
        }
        positionsToCheck.pop();
      }
    }
  }

  return { part1, part1 };
}

}
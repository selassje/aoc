
#include "aoc25/day4.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace {
using aoc25::day4::Input;
using aoc25::day4::Tile;

struct Point
{
  std::size_t x;
  std::size_t y;
};

auto
getNeighbours(const Input& input, Point point)
{
  std::array<Tile, 8> neighbours{};
  std::size_t index = 0;
  for (int dy = -1; dy <= 1; ++dy) {
    for (int dx = -1; dx <= 1; ++dx) {
      if (dx == 0 && dy == 0) {
        continue;
      }
      const std::size_t nx = dx < 0 ? point.x - static_cast<std::size_t>(-dx)
                                    : point.x + static_cast<std::size_t>(dx);
      const std::size_t ny = dx < 0 ? point.y - static_cast<std::size_t>(-dy)
                                    : point.y + static_cast<std::size_t>(dy);
      if (ny < input.size() && nx < input[ny].size()) {
        neighbours[index++] = input[ny][nx];
      } else {
        neighbours[index++] = Tile::Empty;
      }
    }
  }
  return neighbours;
}

auto
getRemovableRolls(const Input& input)
{
  std::vector<Point> removableRolls{};
  for (std::size_t x = 0; x < input[0].size(); ++x) {
    for (std::size_t y = 0; y < input.size(); ++y) {
      const auto neighbours = getNeighbours(input, Point{ x, y });
      const auto rollCount = static_cast<std::size_t>(
        std::count_if(neighbours.begin(), neighbours.end(), [](const Tile t) {
          return t == Tile::Roll;
        }));
      if (input[y][x] == Tile::Roll && rollCount < 4) {
        removableRolls.emplace_back(Point{ x, y });
      }
    }
  }
  return removableRolls;
}

void
removeRolls(Input& input, const std::vector<Point>& rollsToRemove)
{
  for (const auto& point : rollsToRemove) {
    input[point.y][point.x] = Tile::Empty;
  }
}
}
namespace aoc25::day4 {

Result
solve(const Input& input)
{
  auto removableRolls = getRemovableRolls(input);
  const std::uint64_t part1 = removableRolls.size();
  std::uint64_t part2 = part1;
  Input newInput = input;
  while (!removableRolls.empty()) {
    removeRolls(newInput, removableRolls);
    removableRolls = getRemovableRolls(newInput);
    part2 += removableRolls.size();
  }
  return { part1, part2 };
}
}
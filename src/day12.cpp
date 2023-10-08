#include "day12.hpp"

#include <limits>
#include <queue>
#include <unordered_map>
#include <unordered_set>


template<>
struct std::hash<aoc22::day12::Position>
{
  std::size_t operator()(const aoc22::day12::Position p) const noexcept
  {
    const std::size_t highY = static_cast<std::size_t>(p.y)
                              << sizeof(aoc22::day12::IndexType);
    return p.x + highY;
  }
};
namespace aoc22::day12 {

Result
solve(const Input& input)
{
  const auto& grid = input.grid;
  const auto height = grid.size();
  const auto width = grid[0].size();
  const auto maxDistance = std::numeric_limits<decltype(Result::Part1)>::max();

  std::unordered_set<Position> visitedPositions{};
  std::queue<Position> toBeVisitedPositions{};
  std::unordered_map<Position, std::size_t> distances{};

  for (IndexType y = 0; y < height; ++y) {
    for (IndexType x = 0; x < width; ++x) {
      const auto position = Position{ x, y };
      auto& distance = distances[position];
      if (input.startPosition != position) {
        distance = maxDistance;
      }
    }
  }

  auto getNeighbours = [&](const auto& position) {
    std::vector<Position> neighbours{};
    const auto& [x, y] = position;
    if (x > 0) {
      neighbours.emplace_back(Position{ x - 1, y });
    }
    if (x < width - 1) {
      neighbours.emplace_back(Position{ x + 1, y });
    }
    if (y > 0) {
      neighbours.emplace_back(Position{ x, y - 1 });
    }
    if (y < height - 1) {
      neighbours.emplace_back(Position{ x, y + 1 });
    }
    return neighbours;
  };

  auto isStepPossible = [&](const auto& src, const auto& dst) {
    return grid[src.y][src.x] + 1 >= grid[dst.y][dst.x];
  };

  toBeVisitedPositions.push(input.startPosition);
  while (!toBeVisitedPositions.empty()) {
    const auto position = toBeVisitedPositions.front();
    for (const auto& neighbour : getNeighbours(position)) {
      if (isStepPossible(position, neighbour) &&
          visitedPositions.contains(neighbour)) {
      }
    }

    visitedPositions.insert(position);
    toBeVisitedPositions.pop();
  }

  return { distances[input.finalPosition], input.grid.size() };
}

};

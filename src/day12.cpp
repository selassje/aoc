#include "day12.hpp"

#include <algorithm>
#include <cstddef>
#include <deque>
#include <functional>
#include <limits>
#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace aoc22::day12 {
struct PositionInternal : Position
{
  PositionInternal(IndexType x, IndexType y)
    : Position{ x, y } {};
  explicit PositionInternal(const Position& position)
    : Position(position){};
};

}
template<>
struct std::hash<aoc22::day12::PositionInternal>
{
  std::size_t operator()(const aoc22::day12::PositionInternal p) const noexcept
  {
    static_assert(2 * sizeof(aoc22::day12::IndexType) == sizeof(std::size_t));

    const std::size_t highY = static_cast<std::size_t>(p.y)
                              << 8 * sizeof(aoc22::day12::IndexType);
    return p.x + highY;
  }
};

namespace aoc22::day12 {

bool
operator==(const PositionInternal& positionA,
           const PositionInternal& positionB) noexcept
{
  return positionA.x == positionB.x && positionA.y == positionB.y; // NOLINT
}

bool
operator!=(const PositionInternal& positionA,
           const PositionInternal& positionB) noexcept
{
  return !(positionA == positionB);
}

Result
solve(const Input& input)
{
  const auto& grid = input.grid;
  const auto height = grid.size();
  const auto width = grid[0].size();
  const auto maxDistance = std::numeric_limits<decltype(Result::part1)>::max();

  std::unordered_set<PositionInternal> visitedPositions{};
  std::deque<PositionInternal> toBeVisitedPositions{};
  std::unordered_map<PositionInternal, std::size_t> distances{};

  for (IndexType y = 0; y < height; ++y) {
    for (IndexType x = 0; x < width; ++x) {
      const auto position = PositionInternal(x, y);
      auto& distance = distances[position];
      if (static_cast<PositionInternal>(input.finalPosition) != position) {
        distance = maxDistance;
      }
    }
  }

  auto getNeighbours = [&](const auto& position) {
    std::vector<PositionInternal> neighbours{};
    neighbours.reserve(4);
    const auto x = position.x;
    const auto y = position.y;

    if (x > 0) {
      neighbours.emplace_back(x - 1, y);
    }
    if (x < width - 1) {
      neighbours.emplace_back(x + 1, y);
    }
    if (y > 0) {
      neighbours.emplace_back(x, y - 1);
    }
    if (y < height - 1) {
      neighbours.emplace_back(x, y + 1);
    }
    return neighbours;
  };

  auto isStepPossible = [&](const auto& src, const auto& dst) {
    return grid[src.y][src.x] + 1 >= grid[dst.y][dst.x];
  };

  toBeVisitedPositions.push_back(
    static_cast<PositionInternal>(input.finalPosition));
  while (!toBeVisitedPositions.empty()) {
    const auto position = toBeVisitedPositions.front();
    for (const auto& neighbour : getNeighbours(position)) {
      if (isStepPossible(neighbour, position) &&
          !(visitedPositions.contains(neighbour) ||
            std::ranges::find(toBeVisitedPositions, neighbour) !=
              toBeVisitedPositions.end())) {
        const auto canidateLength = distances[position] + 1;
        if (canidateLength < distances[neighbour]) {
          distances[neighbour] = canidateLength;
        }
        toBeVisitedPositions.push_back(neighbour);
      }
    }

    visitedPositions.insert(position);
    toBeVisitedPositions.pop_front();
  }

  auto positionsA =
    distances | std::views::filter([&grid](const auto& position) {
      return grid[position.first.y][position.first.x] == 'a';
    });

  const auto part2 = std::ranges::min_element(
                       positionsA,
                       std::ranges::less{},
                       [](const auto& distance) { return distance.second; })
                       ->second;

  return { distances[static_cast<PositionInternal>(input.startPosition)],
           part2 };
}

};

#include "aoc22/day12.hpp"

#include <algorithm>
#include <cstddef>
#include <deque>
#include <exception>
#include <format>
#include <functional>
#include <limits>
#include <ranges>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

template<>
struct std::hash<aoc22::day12::Position>
{
  std::size_t operator()(const aoc22::day12::Position p) const noexcept
  {
    static_assert(2 * sizeof(aoc22::day12::IndexType) == sizeof(std::size_t));

    const std::size_t highY = static_cast<std::size_t>(p.y)
                              << 8 * sizeof(aoc22::day12::IndexType);
    return p.x + highY;
  }
};

namespace aoc22::day12 {

struct Exception : std::exception
{
  std::string message;
  [[nodiscard]] const char* what() const noexcept override
  {
    return message.c_str();
  }
  explicit Exception(std::string message_)
    : message(std::move(message_))
  {
  }
};

namespace {

void
veryfiInput(const Input& input)
{
  const auto& grid = input.grid;
  static const char* const prefix = "Aoc22::day12::solve";

  const auto height = grid.size();
  if (height == 0) {
    throw Exception{ std::format("{} Grid height is 0 ", prefix) };
  }

  const auto width = grid[0].size();
  if (width == 0) {
    throw Exception{ std::format("{} Grid width is 0 ", prefix) };
  }

  for (std::size_t i = 0; i < height; ++i) {
    if (grid[i].size() != width) {
      throw Exception{ std::format(
        "{} Grid is ill formed : Row 0 has size {} but row {} has size {} ",
        prefix,
        width,
        i,
        grid[i].size()) };
    }
    for (std::size_t j = 0; j < width; ++j) {
      if (grid[i][j] < 'a' || grid[i][j] > 'z') {
        throw Exception{ std::format(
          "{} Grid is ill formed : Value {} at ({},{}) is outside "
          "of the valid range ('a' - 'z') ",
          prefix,
          grid[i][j],
          j,
          i) };
      }
    }
  }
  if (input.startPosition.x >= width || input.startPosition.y >= height) {
    throw Exception{ std::format(
      "{} Input is ill formed : Start position ({},{}) is outside of the "
      "grid ( {} x  {}) ",
      prefix,
      input.startPosition.x,
      input.startPosition.y,
      width,
      height) };
  }

  if (const auto valueAtStart = grid[input.startPosition.y][input.startPosition.x]; valueAtStart != 'a') {
    throw Exception{ std::format("{} Input is ill formed : Value at {} start "
                                 "position ({},{}) should be 'a'",
                                 prefix,
                                 input.startPosition.x,
                                 input.startPosition.y,
                                 valueAtStart) };
  }

  if (input.finalPosition.x >= width || input.finalPosition.y >= height) {
    throw Exception{ std::format(
      "{} Input is ill formed : Final position ({},{}) is outside of the "
      "grid ( {} x  {}) ",
      prefix,
      input.finalPosition.x,
      input.finalPosition.y,
      width,
      height) };
  }
  const auto valueAtEnd = grid[input.finalPosition.y][input.finalPosition.x];
  if (valueAtEnd != 'z') {
    throw Exception{ std::format("{} Input is ill formed : Value at {} final "
                                 "position ({},{}) should be 'z'",
                                 prefix,
                                 input.finalPosition.x,
                                 input.finalPosition.y,
                                 valueAtEnd) };
  }
}
}
Result
solve(const Input& input)
{
  veryfiInput(input);

  const auto& grid = input.grid;
  const auto height = grid.size();
  const auto width = grid[0].size();
  static constexpr auto maxDistance =
    std::numeric_limits<decltype(Result::part1)>::max();

  std::unordered_set<Position> visitedPositions{};
  std::deque<Position> toBeVisitedPositions{};
  std::unordered_map<Position, std::size_t> distances{};

  for (IndexType y = 0; y < height; ++y) {
    for (IndexType x = 0; x < width; ++x) {
      const auto position = Position{ x, y };
      auto& distance = distances[position];
      if (input.finalPosition != position) {
        distance = maxDistance;
      }
    }
  }

  auto isStepPossible = [&](const auto& src, const auto& dst) {
    return grid[src.y][src.x] + 1 >= grid[dst.y][dst.x] &&
           !(visitedPositions.contains(src) ||
             std::ranges::find(toBeVisitedPositions, src) !=
               toBeVisitedPositions.end());
  };

  auto getNeighbours = [&](const auto& position) {
    std::vector<Position> neighbours{};
    neighbours.reserve(4);
    const auto& [x, y] = position;
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

  auto getEligibleNeighbours =
    [&](const auto& position) -> std::vector<Position> { // NOLINT
    auto filtered =
      getNeighbours(position) |
      std::views::filter([&isStepPossible, &position](const auto& n) {
        return isStepPossible(n, position);
      });
    return { filtered.begin(), filtered.end() };
  };

  toBeVisitedPositions.push_back(input.finalPosition);
  while (!toBeVisitedPositions.empty()) {
    const auto position = toBeVisitedPositions.front();
    for (const auto& neighbour : getEligibleNeighbours(position)) {
      const auto canidateLength = distances[position] + 1;
      distances[neighbour] = std::min(canidateLength, distances[neighbour]);
      toBeVisitedPositions.push_back(neighbour);
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

  return { distances[input.startPosition], part2 };
}
};

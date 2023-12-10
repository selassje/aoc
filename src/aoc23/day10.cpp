#include "aoc23/day10.hpp"
#include <deque>
#include <optional>
#include <set>
#include <utility>

namespace aoc23::day10 {

struct Position
{
  std::size_t x;
  std::size_t y;
  auto operator<=>(const Position&) const noexcept = default;
};

Position
findStart(const Input& input)
{
  for (std::size_t y = 0; y < input.size(); ++y) {
    for (std::size_t x = 0; x < input[y].size(); ++x) {
      if (input[y][x] == PipeType::Start) {
        return { x, y };
      }
    }
  }
  return { 0, 0 };
}

auto
getNeighbours(const Position& position, PipeType pipeType, const Input& input)
{
  std::optional<std::pair<Position, Position>> neighbours = std::nullopt;
  using enum PipeType;
  const auto& [x, y] = position;
  const auto height = input.size();
  const auto width = input[0].size();
  switch (pipeType) {
    case NorthSouth:
      if (y > 0 && y < height - 1) {
        neighbours = { { x, y - 1 }, { x, y + 1 } };
      }
      break;
    case EastWest:
      if (x > 0 && x < width - 1) {
        neighbours = { { x + 1, y }, { x - 1, y } };
      }
      break;
    case NorthEast:
      if (x < width - 1 && y > 0) {
        neighbours = { { x, y - 1 }, { x + 1, y } };
      }
      break;
    case NorthWest:
      if (x > 0 && y > 0) {
        neighbours = { { x, y - 1 }, { x - 1, y } };
      }
      break;
    case SouthWest:
      if (x > 0 && y < height - 1) {
        neighbours = { { x, y + 1 }, { x - 1, y } };
      }
      break;
    case SouthEast:
      if (x < width - 1 && y < height - 1) {
        neighbours = { { x, y + 1 }, { x + 1, y } };
      }
      break;
    default:
      break;
  }
  return neighbours;
}

std::size_t
solvePart1(const Input& input, const Position& start, PipeType startPipe)
{
  std::size_t loopSize = 0;
  bool loopFound = false;

  auto neighbours = getNeighbours(start, startPipe, input);
  if (neighbours) {
    const auto& [firstNeighbour, _] = *neighbours;
    const auto nextNeighbours = getNeighbours(
      firstNeighbour, input[firstNeighbour.y][firstNeighbour.x], input);
    if (nextNeighbours) {
      std::set<Position> visited{};
      visited.insert(firstNeighbour);
      std::deque<Position> toBeVisited{};
      toBeVisited.push_back(nextNeighbours->first != start
                              ? nextNeighbours->first
                              : nextNeighbours->second);
      loopSize = 1;
      while (!toBeVisited.empty()) {
        ++loopSize;
        const auto current = toBeVisited.front();
        toBeVisited.pop_front();
        if (current == start) {
          loopFound = true;
          break;
        }
        visited.insert(current);
        neighbours = getNeighbours(current, input[current.y][current.x], input);
        if (neighbours) {
          if (!visited.contains(neighbours->first)) {
            toBeVisited.push_back(neighbours->first);
          }
          if (!visited.contains(neighbours->second)) {
            toBeVisited.push_back(neighbours->second);
          }
        } else {
          break;
        }
      }
    }
  }
  return loopFound ? loopSize / 2 : 0;
}

Result
solve(const Input& input)
{
  const auto start = findStart(input);

  std::size_t part1 = 0;
  for (int i = 0; i <= static_cast<int>(PipeType::SouthEast); ++i) {
    const auto pipeType = static_cast<PipeType>(i);
    part1 = std::max(part1, solvePart1(input, start, pipeType));
  }
  return { part1, part1 };
}

}
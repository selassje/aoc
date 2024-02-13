#include "aoc23/day10.hpp"
#include <deque>
#include <map>
#include <optional>
#include <limits>
#include <queue>
#include <set>
#include <utility>
#include <variant>

namespace aoc23::day10 {

struct Position
{
  std::size_t x;
  std::size_t y;
  auto operator<=>(const Position&) const noexcept = default;
};

struct Gap {
  Position x;
  Position y;
  auto operator<=>(const Gap&) const noexcept = default;
};

using GroundOrGap = std::variant<Position,Gap>;

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
findPotentialConnections(const Input& input, const Position& position)
{
  std::optional<std::pair<Position, Position>> potentialConnections =
    std::nullopt;
  using enum PipeType;
  const auto& [x, y] = position;
  const auto height = input.size();
  const auto width = input[0].size();
  const auto pipeType = input[position.y][position.x];
  switch (pipeType) {
    case NorthSouth:
      if (y > 0 && y < height - 1) {
        potentialConnections = { { x, y - 1 }, { x, y + 1 } };
      }
      break;
    case EastWest:
      if (x > 0 && x < width - 1) {
        potentialConnections = { { x + 1, y }, { x - 1, y } };
      }
      break;
    case NorthEast:
      if (x < width - 1 && y > 0) {
        potentialConnections = { { x, y - 1 }, { x + 1, y } };
      }
      break;
    case NorthWest:
      if (x > 0 && y > 0) {
        potentialConnections = { { x, y - 1 }, { x - 1, y } };
      }
      break;
    case SouthWest:
      if (x > 0 && y < height - 1) {
        potentialConnections = { { x, y + 1 }, { x - 1, y } };
      }
      break;
    case SouthEast:
      if (x < width - 1 && y < height - 1) {
        potentialConnections = { { x, y + 1 }, { x + 1, y } };
      }
      break;
    default:
      break;
  }
  return potentialConnections;
}

bool
isConnection(const Input& input,
             const Position& position1,
             const Position& position2)
{
  const auto potentialConnections1 = findPotentialConnections(input, position1);
  const auto potentialConnections2 = findPotentialConnections(input, position2);

  if (potentialConnections1 && potentialConnections2) {
    const auto& [f1, s1] = *potentialConnections1;
    const auto& [f2, s2] = *potentialConnections2;
    return f1 == f2 || f1 == s2 || s1 == f2 || s1 == s2;
  }
  return false;
}

void
setStartPipeType(Input& input, const Position& startPosition)
{
  for (int i = 0; i <= static_cast<int>(PipeType::SouthEast); ++i) {
    const auto pipeType = static_cast<PipeType>(i);
    input[startPosition.y][startPosition.x] = pipeType;
    const auto connections = findPotentialConnections(input, startPosition);
    if (connections) {
      if (isConnection(input, startPosition, connections->first) &&
          isConnection(input, startPosition, connections->second)) {
        break;
      }
    }
  }
}


auto
findLoop(Input& input)
{
  const auto startPosition = findStart(input);
  setStartPipeType(input, startPosition);
  std::vector loop{ startPosition };
  while (true) {
    const auto currentPosition = loop.back();
    const auto connections = *findPotentialConnections(input, currentPosition);
    if (std::ranges::find(loop, connections.first) == loop.end()) {
      loop.push_back(connections.first);
    } else if (std::ranges::find(loop, connections.second) == loop.end()) {
      loop.push_back(connections.second);
    } else {
      break;
    }
  }
  return loop;
}

auto getNeighbours(const GroundOrGap &groundOrGap, const Input& input) {
    std::vector<GroundOrGap> neighbours{};
    const auto& [x,y] = std::get<Position>(groundOrGap);
    if (x > 0) {
      neighbours.emplace_back(Position{x - 1, y});
    }
    if (x < input[0].size() - 1) {
      neighbours.emplace_back(Position{x + 1, y});
    }
    if (y > 0) {
      neighbours.emplace_back(Position{x, y - 1});
    }
    if (y < input.size() - 1) {
      neighbours.emplace_back(Position{x, y + 1});
    }
    return neighbours;
}

bool isOnTheEdge(const GroundOrGap &groundOrGap, const Input &input) {
    const auto& [x,y] = std::get<Position>(groundOrGap);
    return x == 0 || x == input[0].size() - 1 || y == 0 || y == input.size() - 1;
}

Result
solve(const Input& inputOrg)
{
  Input input = inputOrg; 
  const auto loop = findLoop(input);
  const std::size_t part1 = loop.size() / 2;

  using Positions = std::set<Position>;

  Positions unCheckedGroundTiles{};
  for (std::size_t y = 0; y < input.size(); ++y) {
    for (std::size_t x = 0; x < input[y].size(); ++x) {
      if (input[y][x] == PipeType::Ground) {
        unCheckedGroundTiles.insert({x,y});
      }
    }
  }

  std::size_t part2 = 0;

  struct Range {
    std::size_t min;
    std::size_t max;
  };

  std::map<std::size_t, Range> yToXs{};
  Range yS{std::numeric_limits<std::size_t>::max(),0}; 
  for(const auto& pipe : loop) {
      yS.max = std::max(yS.max,pipe.y);
      yS.min = std::min(yS.min,pipe.y);

      if ( yToXs.find(pipe.y) == yToXs.end()) {
        yToXs[pipe.y] = {std::numeric_limits<std::size_t>::max(),0};
      }

      yToXs[pipe.y].min = std::min(yToXs[pipe.y].min, pipe.x);
      yToXs[pipe.y].max = std::max(yToXs[pipe.y].max, pipe.x);

  } 

  for ( std::size_t y = yS.min ; y <= yS.max; ++y) {
      for ( std::size_t x = yToXs[y].min ; x <= yToXs[y].max; ++x ) {
        if ( input[y][x] == PipeType::Ground ) {
            ++part2;
        }
      }    
  }

  part2 = 0;
  while(!unCheckedGroundTiles.empty()) {
      bool isOutside = false;
      const auto first = *unCheckedGroundTiles.begin();
      std::queue<GroundOrGap> toBeVisited{};
      std::set<GroundOrGap> visited{};
      toBeVisited.push(first);
      while(!toBeVisited.empty()) {
          const auto current = toBeVisited.front();
          visited.insert(current);
          toBeVisited.pop();
          if ( isOnTheEdge(current, input)) {
              isOutside = true;
          }
          const auto neighbours = getNeighbours(current,input);
          for ( const auto& n : neighbours) {
            const auto& position = std::get<Position>(n);
            if ( !visited.contains(n) && input[position.y][position.x] == PipeType::Ground ) {
                toBeVisited.push(n);
            }
          }
     }
     for ( const auto& v : visited) {
        if ( v.index() == 0) {
            const auto ground = std::get<Position>(v);
            unCheckedGroundTiles.erase(ground);
            if  ( isOutside) {
                ++part2;
            }
        }
     }
  }

  return { part1, part2 };
}

}
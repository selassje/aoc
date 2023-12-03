#include "aoc23/day3.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <map>
#include <optional>

namespace aoc23::day3 {

struct Position
{
  std::size_t x;
  std::size_t y;
  auto operator<=>(const Position&) const noexcept = default;
};

struct Number
{
  Position startPosition;
  std::size_t length;
  std::size_t value;
};

struct GearInfo
{
  std::size_t gearRatio{ 1 };
  std::size_t partNumberCount{ 0 };
};

auto
parseInput(const Input& input)
{
  std::vector<Number> numbers{};
  const auto width = input[0].size();
  for (std::size_t y = 0; y < input.size(); ++y) {
    const auto& line = input[y];
    std::optional<std::size_t> startX = std::nullopt;
    auto getValue = [&](const std::size_t endX) -> Number {
      const auto subStr = line.substr(*startX, endX - *startX + 1);
      const auto value = std::strtoull(subStr.c_str(), nullptr, 10);
      return { { *startX, y }, endX - *startX + 1, value };
    };

    for (std::size_t x = 0; x < width; ++x) {
      if (std::isdigit(line[x])) {
        if (!startX) {
          startX = x;
        }
        if (x == width - 1) {
          const Number number = getValue(x);
          numbers.push_back(number);
        }
      } else {
        if (startX) {
          const Number number = getValue(x - 1);
          numbers.push_back(number);
          startX = std::nullopt;
        }
      }
    }
  }
  return numbers;
}

Result
solve(const Input& input)
{
  const auto height = input.size();
  const auto width = input[0].size();

  std::size_t part1 = 0;
  auto getSymbol = [&](const Position& position) {
    if (position.x >= width || position.y >= height) {
      return '.';
    }
    return input[position.y][position.x];
  };

  std::map<Position, GearInfo> gearInfoMap{};
  for (const Number& number : parseInput(input)) {
    const auto& [startX, y] = number.startPosition;
    const auto endX = startX + number.length - 1;
    std::vector<Position> adjacent = {
      { startX - 1, y },     { endX + 1, y },     { startX - 1, y - 1 },
      { startX - 1, y + 1 }, { endX + 1, y - 1 }, { endX + 1, y + 1 }
    };

    for (auto x = startX; x <= endX; ++x) {
      adjacent.push_back({ x, y - 1 });
      adjacent.push_back({ x, y + 1 });
    }

    for (const auto& position : adjacent) {
      const auto symbol = getSymbol(position);
      if (symbol != '.') {
        part1 += number.value;
        if (symbol == '*') {
          gearInfoMap[position].partNumberCount += 1;
          gearInfoMap[position].gearRatio *= number.value;
        }
        break;
      }
    }
  }
  std::size_t part2 = 0;
  std::ranges::for_each(gearInfoMap, [&part2](const auto& pair) {
    if (pair.second.partNumberCount == 2) {
      part2 += pair.second.gearRatio;
    }
  });

  return { part1, part2 };
}
}

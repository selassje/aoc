#include "inputs.hpp"
#include "parsing.hpp"

#include <catch2/catch_test_macros.hpp>

import aoc25.day12;
import std;

using aoc25::day12::Input;
using aoc25::day12::Region;
using aoc25::day12::Shape;
namespace {
auto
readInput(std::string_view path)
{
  Input input{};
  auto fileReader = inputs::FileReader{ path };
  Shape shape{};
  bool readingShape = true;
  while (auto line = fileReader.readLine()) {
    if (!line->empty()) {
      if (line->contains(':')) {
        if (line->contains('x')) {
          readingShape = false;
          Region region{};
          std::ranges::replace(*line, ':', ' ');
          std::ranges::replace(*line, 'x', ' ');
          const auto numbers = inputs::parseStringDynamic<std::uint64_t>(*line);
          region.width = numbers[0];
          region.height = numbers[1];
          region.shapesCount =
            std::vector<std::uint64_t>(numbers.begin() + 2, numbers.end());
          input.regions.push_back(region);

        } else {
          std::ranges::replace(*line, ':', ' ');
          const auto [id] = inputs::parseLine<std::uint64_t>(*line);
          shape.id = id;
          shape.grid.clear();
          continue;
        }
      }
      if (readingShape) {
        auto tiles =
          std::views::transform(*line,
                                [](char c) {
                                  return c == '#' ? aoc25::day12::Tile::Full
                                                  : aoc25::day12::Tile::Empty;
                                }) |
          std::ranges::to<std::vector>();
        shape.grid.push_back(tiles);
      }

    } else {
      input.shapes.push_back(shape);
    }
  }
  return input;
}
}

TEST_CASE("Aoc25 Day12 Example", "[AoC25_Day12]")
{
  /// const auto input = readInput(inputs::day12::EXAMPLE);
  // const auto result = aoc25::day12::solve(input); //NOLINT
  //  REQUIRE(result == 2);
}

TEST_CASE("Aoc25 Day12 Input", "[AoC25_Day12]")
{
  const auto input = readInput(inputs::day12::INPUT);
  const auto result = aoc25::day12::solve(input);
  REQUIRE(result == 485);
}
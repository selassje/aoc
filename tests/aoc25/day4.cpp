
#include "day4.hpp"
#include "inputs.hpp"
#include "parsing.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <string_view>
#include <vector>

using aoc25::day4::Input;
using aoc25::day4::Tile;
using inputs::FileReader;
namespace {
auto
readInput(std::string_view path)
{
  Input input{};
  FileReader fileReader{ path };
  while (const auto& line = fileReader.readLine()) {
    const auto rawTiles = inputs::parseStringDynamic<unsigned char>(*line);
    std::vector<Tile> tiles{};
    for ( const auto c : rawTiles) {
      if ( c == '.' ) {
        tiles.emplace_back(Tile::Empty);
      } else {
        tiles.emplace_back(Tile::Roll);
      }
    }
    input.emplace_back(tiles);
  }
  return input;
}
}

TEST_CASE("Aoc25 Day4 Example", "[AoC25_Day4]")
{
  const auto input = readInput(inputs::day4::EXAMPLE);
  const auto [part1, part2] = aoc25::day4::solve(input);
  REQUIRE(part1 == 13);
  REQUIRE(part2 == 13);
}

TEST_CASE("Aoc25 Day4 Input", "[AoC25_Day4]")
{
 // const auto input = readInput(inputs::day3::INPUT);
  //const auto [part1, part2] = aoc25::day3::solve(input);
 // REQUIRE(part1 == 17445);
 // REQUIRE(part2 == 173229689350551);
}
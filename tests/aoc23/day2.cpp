
#include "aoc23/day2.hpp"
#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <fstream>
#include <string>
#include <string_view>

using aoc23::day2::Draw;
using aoc23::day2::Game;
using aoc23::day2::Input;

Input
readInput(const std::string_view path)
{
  std::ifstream ifs{ path.data() };
  Input input{};
  std::string line{};
  auto split = [](const std::string& string, const char delimeter) {
    std::vector<std::string> parts{};
    std::size_t searchOffset = 0;
    std::size_t nextDelimeter = string.find(delimeter);
    while (nextDelimeter != std::string::npos) {
      const auto part =
        string.substr(searchOffset, nextDelimeter - searchOffset + 1);
      parts.push_back(part);
      searchOffset = nextDelimeter;
      nextDelimeter = string.find(delimeter, searchOffset + 1);
    }
    return parts;
  };
  while (std::getline(ifs, line)) {
    Game game{};
    const auto drawsStr = split(line, ':')[1];
    for (const auto& drawStr : split(drawsStr, ';')) {
      Draw draw{};
      for (const auto& cubeStr : split(drawStr, ',')) {
        const auto words = split(cubeStr, ' ');
        const auto count = std::strtoull(words[0].c_str(), nullptr, 10);
        const auto& colour = words[1];
        if (colour == "red") {
          draw.redCount = count;
        }
        if (colour == "green") {
          draw.greenCount = count;
        }
        if (colour == "blue") {
          draw.blueCount = count;
        }
      }
      game.push_back(draw);
    }
    input.push_back(game);
    game.clear();
  }
  return input;
}

TEST_CASE("Aoc23 Day2 Example", "[AoC23_Day2]")
{
  const auto input = readInput(inputs::day2::EXAMPLE);
  const auto& [part_1, _] = aoc23::day2::solve(input);
  REQUIRE(part_1 == 8);
}
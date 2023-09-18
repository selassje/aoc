#include "day10.hpp"
#include "inputs.hpp"
#include "tests_main.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

using aoc22::day10::AddX;
using aoc22::day10::Input;
using aoc22::day10::Instruction;
using aoc22::day10::Noop;

Input
readInput(const std::string_view path)
{
  Input input{};

  std::vector<std::string> words{};
  auto split = [&words](auto& iss) {
    words.clear();
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(),
              std::back_inserter(words));
  };

  std::ifstream ifs{ path.data() };
  std::string line{};
  const std::string noopIntruction = "noop";
  const std::string addXInstruction = "addx";

  while (std::getline(ifs, line)) {
    Instruction instruction{};
    std::istringstream iss{ line };
    split(iss);
    if (words[0] == addXInstruction) {
      instruction = aoc22::day10::AddX{ std::stoll(words[1]) };
    } else if (words[0] == noopIntruction) {
      instruction = aoc22::day10::Noop{};
    }
    input.push_back(instruction);
  }
  return input;
}

void
compareRenders(const auto& r1, const auto r2)
{
  for (std::size_t i = 0; i < aoc22::day10::CRT_HEIGHT; ++i) {
    for (std::size_t j = 0; j < aoc22::day10::CRT_WIDTH; ++j) {
      REQUIRE(r1[i][j] == r2[i][j]);
    }
  }
}

TEST_CASE("Day10 Example", "[Day10]")
{
  const auto input = readInput(inputs::day10::EXAMPLE);
  const auto& [part1, part2] = aoc22::day10::solve(input);
  REQUIRE(part1 == 13140);
  const std::vector<std::string> expectedPart2 = {
    "##..##..##..##..##..##..##..##..##..##..",
    "###...###...###...###...###...###...###.",
    "####....####....####....####....####....",
    "#####.....#####.....#####.....#####.....",
    "######......######......######......####",
    "#######.......#######.......#######....."
  };
  compareRenders(expectedPart2, part2);
}

TEST_CASE("Day10 Input", "[Day10]")
{
  const auto input = readInput(inputs::day10::INPUT);
  const auto& [part1, part2] = aoc22::day10::solve(input);

  REQUIRE(part1 == 14560);
  const std::vector<std::string> expectedPart2 = {
    "####.#..#.###..#..#.####.###..#..#.####.",
    "#....#.#..#..#.#..#.#....#..#.#..#....#.",
    "###..##...#..#.####.###..#..#.#..#...#..",
    "#....#.#..###..#..#.#....###..#..#..#...",
    "#....#.#..#.#..#..#.#....#....#..#.#....",
    "####.#..#.#..#.#..#.####.#.....##..####."
  };
  compareRenders(expectedPart2, part2);
}

#include "day10.hpp"
#include "tests_main.hpp"
#include "inputs.hpp"

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
      instruction = aoc22::day10::AddX{std::stoll(words[1])};
    } else if (words[0] == noopIntruction) {
      instruction = aoc22::day10::Noop{};
    }
    input.push_back(instruction);
  }
  return input;
}

TEST_CASE("Day10 Example", "[Day10]")
{
  const auto input = readInput(inputs::day10::EXAMPLE);
  const auto& [part_1, part_2] = aoc22::day10::solve(input);
  REQUIRE(part_1 == 13140);
  REQUIRE(part_2 == 13140);
}




#include "day11.hpp"
#include "inputs.hpp"
#include "tests_main.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

using aoc22::day11::Input;

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
  //  Instruction instruction{};
    std::istringstream iss{ line };
    split(iss);
    if (words[0] == addXInstruction) {
   //   instruction = aoc22::day10::AddX{ std::stoll(words[1]) };
    } else if (words[0] == noopIntruction) {
     // instruction = aoc22::day10::Noop{};
    }
    //input.push_back(instruction);
  }
  return input;
}

TEST_CASE("Day11 Example", "[Day11]")
{
  const auto input = readInput(inputs::day11::EXAMPLE);
  const auto& [part_1, part_2] = aoc22::day11::solve(input);
  REQUIRE(part_1 == 10605);
  REQUIRE(part_2 == 10605);
}
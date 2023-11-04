#include "day19.hpp"

#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <fstream>
#include <regex>
#include <string>
#include <string_view>

using aoc22::day19::Blueprint;
using aoc22::day19::Input;

Input
readInput(const std::string_view path_)
{
  std::ifstream ifs{ path_.data() };
  Input input{};
  Blueprint blueprint{}; // NOLINT

  const std::regex idRegex{ "Blueprint (\\d+)" };
  const std::regex oreRegex{ "(\\d+) ore" };
  const std::regex clayRegex{ "(\\d+) clay" };
  const std::regex obsidianRegex{ "(\\d+) obsidian" };

  std::string line{};

  auto regex = [&](const auto& rgx) {
    std::vector<int> numbers{};
    std::smatch matches{};
    std::string::const_iterator searchStart( line.cbegin() );
    while (std::regex_search(searchStart, line.cend(), matches, rgx)) {
      numbers.push_back(std::stoi(matches[1].str()));
      searchStart = matches.suffix().first;
    }
    return numbers;
  };

  while (std::getline(ifs, line)) {
    blueprint.id = regex(idRegex)[0];
    const auto ores = regex(oreRegex);
    blueprint.oreRobotCost.ore = ores[0];
    blueprint.clayRobotCost.ore = ores[1];
    blueprint.obsidianRobotCost.ore = ores[2];
    blueprint.geodeRobotCost.ore = ores[3];
    blueprint.obsidianRobotCost.clay = regex(clayRegex)[0];
    blueprint.geodeRobotCost.obsidian = regex(obsidianRegex)[0];
    input.push_back(blueprint);
  }
  return input;
}

TEST_CASE("Day19 Example", "[Day19]")
{
  const auto input = readInput(inputs::day19::EXAMPLE);
  const auto& [part_1, part_2] = aoc22::day19::solve(input);
  REQUIRE(part_1 == 33);
  REQUIRE(part_2 == 33);
}

TEST_CASE("Day19 Input", "[Day19]")
{
 // const auto input = readInput(inputs::day19::INPUT);
 // const auto& [part_1, part_2] = aoc22::day19::solve(input);
 // REQUIRE(part_1 == 33);
 // REQUIRE(part_2 == 33);
}

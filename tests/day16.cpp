
#include "day16.hpp"
#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <regex>
#include <string>
#include <string_view>

using aoc22::day16::Input;
using aoc22::day16::Valve;

Input
readInput(const std::string_view path)
{
  std::ifstream ifs{ path.data() };
  Input input{};
  Valve valve{};

  const std::regex regexFlowRate("(\\d+)");
  const std::regex regexValveName("([A-Z][A-Z])");
  const auto endRegexIt = std::sregex_iterator();

  std::string line{};
  while (std::getline(ifs, line)) {
    if (!line.empty()) {
      const std::size_t flowRate = std::stoull(
        std::sregex_iterator(line.begin(), line.end(), regexFlowRate)->str());
      auto namesIt =
        std::sregex_iterator(line.begin(), line.end(), regexValveName);
      valve.flowRate = flowRate;
      valve.name = namesIt->str();
      while (++namesIt != endRegexIt) {
        valve.connectedValves.emplace_back(namesIt->str());
      }
      input.push_back(valve);
      valve.connectedValves.clear();
    }
  }
  return input;
}


TEST_CASE("Day16 Example", "[Day16]")
{
  const auto input = readInput(inputs::day16::EXAMPLE);
  const auto& [part_1, part_2] = aoc22::day16::solve(input);
  REQUIRE(part_1 == 1651);
  REQUIRE(part_2 == 1707);
}

TEST_CASE("Day16 Input", "[Day16]")
{
  const auto input = readInput(inputs::day16::INPUT);
  //const auto& [part_1, part_2] = aoc22::day16::solve(input);
 // REQUIRE(part_1 == 1673);
 // REQUIRE(part_2 == 1673);
}
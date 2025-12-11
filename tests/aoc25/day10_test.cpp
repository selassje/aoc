#include "inputs.hpp"
#include "parsing.hpp"

#include <catch2/catch_test_macros.hpp>

#include <regex>

import aoc25.day10;
import std;

using aoc25::day10::ButtonWiring;
using aoc25::day10::ButtonWirings;
using aoc25::day10::Input;
using aoc25::day10::Joltages;
using aoc25::day10::Light;
using aoc25::day10::Lights;
using aoc25::day10::Machine;

namespace {
auto
readInput(std::string_view path)
{
  Input input{};
  auto fileReader = inputs::FileReader{ path };
  while (auto line = fileReader.readLine()) {

    std::regex regex(
      R"(^\s*\[([^\]]*)\]\s*((?:\([^)]*\)\s*)+)\{([^}]*)\}\s*$)");
    std::smatch match;

    std::regex_match(*line, match, regex);

    std::string lightsStr = match[1];
    std::string buttonsStr = match[2];
    std::string joltagesStr = match[3];

    Lights lights;
    for (char c : lightsStr) {
      if (c == '#') {
        lights.push_back(Light::On);
      } else {
        lights.push_back(Light::Off);
      }
    }
    ButtonWirings wirings{};

    std::regex buttonsRe(R"(\(([^)]*)\))");
    auto begin =
      std::sregex_iterator(buttonsStr.begin(), buttonsStr.end(), buttonsRe);
    auto end = std::sregex_iterator();

    for (auto it = begin; it != end; ++it) {
      std::string content = (*it)[1];
      std::replace(content.begin(), content.end(), ',', ' ');
      const auto wiring = inputs::parseStringDynamic<std::uint64_t>(content);
      wirings.push_back(wiring);
    }

    std::replace(joltagesStr.begin(), joltagesStr.end(), ',', ' ');
    const auto joltages =
      inputs::parseStringDynamic<std::uint64_t>(joltagesStr);
    input.push_back(Machine{ lights, wirings, joltages });
  }
  return input;
}
}

TEST_CASE("Aoc25 Day10 Example", "[AoC25_Day10]")
{
  const auto input = readInput(inputs::day10::EXAMPLE);
  const auto [part1, part2] = aoc25::day10::solve(input);
  REQUIRE(part1 == 7);
  REQUIRE(part2 == 33);
}

TEST_CASE("Aoc25 Day10 Input", "[AoC25_Day10]")
{
 // const auto input = readInput(inputs::day10::INPUT);
 // const auto [part1, part2] = aoc25::day10::solve(input);
//  REQUIRE(part1 == 535);
 // REQUIRE(part2 == 535);
}

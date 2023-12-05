

#include "aoc23/day5.hpp"
#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

using aoc23::day5::Input;
using aoc23::day5::Map;
using aoc23::day5::Range;

auto
split(const std::string& string, const char delimeter)
{
  std::vector<std::string> parts{};
  std::size_t searchOffset = 0;
  std::size_t nextDelimeter = string.find(delimeter);
  while (nextDelimeter != std::string::npos) {
    const auto part = string.substr(searchOffset, nextDelimeter - searchOffset);
    parts.push_back(part);
    searchOffset = nextDelimeter + 1;
    nextDelimeter = string.find(delimeter, searchOffset);
  }
  const auto lastPart =
    string.substr(searchOffset, string.size() - searchOffset);
  parts.push_back(lastPart);
  return parts;
};

auto
readNumbers(const std::string& string)
{
  std::vector<std::size_t> numbers{};
  const auto stringRepl = std::regex_replace(string, std::regex("  "), " ");
  const auto numbersStr = split(stringRepl, ' ');
  for (const auto& numberStr : numbersStr) {
    const auto number = std::strtoull(numberStr.c_str(), nullptr, 10);
    numbers.push_back(number);
  }
  return numbers;
}

Input
readInput(const std::string_view path)
{
  std::ifstream ifs{ path.data() };
  Input input{};

  auto getMap = [&input](const std::string& mapHeader) -> Map& {
    if (mapHeader == "seed-to-soil map:") {
      return input.seedToSoil;
    }
    if (mapHeader == "soil-to-fertilizer map:") {
      return input.soilToFertilizer;
    }
    if (mapHeader == "fertilizer-to-water map:") {
      return input.fertilizerToWater;
    }
    if (mapHeader == "water-to-light map:") {
      return input.waterToLight;
    }
    if (mapHeader == "light-to-temperature map:") {
      return input.lightToTemp;
    }
    if (mapHeader == "temperature-to-humidity map:") {
      return input.tempToHumidity;
    }
    return input.humidityToToLocation;
  };

  std::string line{};
  while (std::getline(ifs, line)) {
    if (!line.empty()) {
      if (line.starts_with("seeds")) {
        const auto seedsStr = split(line, ':')[1];
        input.seeds = readNumbers(seedsStr.substr(1));
      } else {
        auto& map = getMap(line);
        while (std::getline(ifs, line)) {
          if (!line.empty()) {
            const auto numbers = readNumbers(line);
            Range range{ numbers[0], numbers[1], numbers[2] };
            map.push_back(range);
          } else {
            break;
          }
        }
      }
    }
  }
  return input;
}

TEST_CASE("Aoc23 Day5 Example", "[AoC23_Day5]")
{
  const auto input = readInput(inputs::day5::EXAMPLE);
  const auto& [part1, part2] = aoc23::day5::solve(input);
  REQUIRE(part1 == 35);
  REQUIRE(part2 == 35);
}

TEST_CASE("Aoc23 Day5 Input", "[AoC23_Day5]")
{
  const auto input = readInput(inputs::day5::INPUT);
  const auto& [part1, part2] = aoc23::day5::solve(input);
  REQUIRE(part1 == 107430936);
  REQUIRE(part2 == 107430936);
}

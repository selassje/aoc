
#include "day15.hpp"
#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <regex>
#include <string>
#include <string_view>

using aoc22::day15::Input;
using aoc22::day15::Record;

namespace {

Input
readInput(const std::string_view path_)
{
  std::ifstream ifs{ path_.data() };
  Input input{};
  Record record{};

  const std::regex regex("(\\-?\\d+)");
  const auto endRegexIt = std::sregex_iterator();

  std::string line{};
  while (std::getline(ifs, line)) {
    if (!line.empty()) {
      auto numbersBegin = std::sregex_iterator(line.begin(), line.end(), regex);
      for (std::size_t i = 0; i < 4; ++i) {
        const auto number = std::stoi(numbersBegin->str());
        if (i == 0) {
          record.sensor.x = number;
        }
        if (i == 1) {
          record.sensor.y = number;
        }
        if (i == 2) {
          record.beacon.x = number;
        }
        if (i == 3) {
          record.beacon.y = number;
        }
        ++numbersBegin;
      }
      input.push_back(record);
    }
  }
  return input;
}
}
namespace aoc22::day15 {

template<typename std::int32_t, typename std::int32_t>
Result
solve(const Input&); // NOLINT

};

TEST_CASE("Day15 Example", "[Day15]")
{
  static constexpr std::int32_t row = 10;
  static constexpr std::int32_t limit = 20;
  const auto input = readInput(inputs::day15::EXAMPLE);
  const auto& [part_1, part_2] = aoc22::day15::solve<row, limit>(input);
  REQUIRE(part_1 == 26);
  REQUIRE(part_2 == 56000011);
}

TEST_CASE("Day15 Input", "[Day15]")
{
  const auto input = readInput(inputs::day15::INPUT);
  const auto& [part_1, part_2] = aoc22::day15::solve(input);
  REQUIRE(part_1 == 5838453);
  REQUIRE(part_2 == 12413999391794);
}
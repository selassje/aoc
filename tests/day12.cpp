
#include "day12.hpp"
#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <fstream>
#include <string>
#include <string_view>
#include <vector>

using aoc22::day12::Grid;
using aoc22::day12::IndexType;
using aoc22::day12::Input;

Input
readInput(const std::string_view path)
{
  Input input{};
  std::ifstream ifs{ path.data() };
  std::string line{};

  IndexType y = 0;
  while (std::getline(ifs, line)) {
    std::vector<char> row(line.size(), -1);
    for (IndexType x = 0; x < line.size(); ++x) {
      auto c = line[x];
      if (c == 'S') {
        input.startPosition = { x, y };
        c = 'a';
      }
      if (c == 'E') {
        input.finalPosition = { x, y };
        c = 'z';
      }
      row[x] = c;
    }
    input.grid.push_back(row);
    ++y;
  }
  return input;
}

TEST_CASE("Day12 Example", "[Day12]")
{
  const auto input = readInput(inputs::day12::EXAMPLE);
  const auto& [part_1, part_2] = aoc22::day12::solve(input);
  REQUIRE(part_1 == 31);
  REQUIRE(part_2 == 29);
}

TEST_CASE("Day12 Input", "[Day12]")
{
  const auto input = readInput(inputs::day12::INPUT);
  const auto& [part_1, part_2] = aoc22::day12::solve(input);
  REQUIRE(part_1 == 370);
  REQUIRE(part_2 == 363);
}

#ifdef ENABLE_FUZZ_TESTS
#include <exception>
#include <fuzztest/fuzztest.h>

void
fuzzTest(const Input& input)
{
  try {
    aoc22::day12::solve(input);
  } catch (const std::exception& e) { // NOLINT
  }
}

FUZZ_TEST(Day12FuzzTest, fuzzTest);

#endif
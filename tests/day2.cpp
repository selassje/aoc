#include "day2.hpp"
#include "inputs.hpp"

#include "tests_main.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using aoc22::day2::Guide;

using enum aoc22::day2::Move;

Guide
readInput(const std::string_view path)
{
  std::ifstream ifs{ path.data() };

  Guide input;

  std::string line{};
  while (std::getline(ifs, line)) {
    unsigned char oponent{};
    unsigned char you{};
    std::istringstream iss{ line };
    iss >> oponent >> you;

    auto converToMove = [](unsigned char move) {
      switch (move) {
        case 'A':
        case 'X':
          return Rock;
        case 'B':
        case 'Y':
          return Paper;
        case 'C':
        case 'Z':
          return Sciscors;
        default:            // GCOVR_EXCL_LIN
          std::terminate(); // GCOVR_EXCL_LIN
      }
    };
    input.emplace_back(
      std::make_pair(converToMove(oponent), converToMove(you)));
  }
  return input;
}

TEST_CASE("Day2 Example", "[Day2]")
{
  const auto input =
    Guide{ { Rock, Paper }, { Paper, Rock }, { Sciscors, Sciscors } };
  const auto [part_1, part_2] = aoc22::day2::solve(input);
  REQUIRE(part_1 == 15);
  REQUIRE(part_2 == 12);
}

TEST_CASE("Day2 Input", "[Day2]")
{
  const auto input = readInput(inputs::day2::INPUT);
  const auto [part_1, part_2] = aoc22::day2::solve(input);
  REQUIRE(part_1 == 11767);
  REQUIRE(part_2 == 13886);
}

#ifdef ENABLE_FUZZ_TESTS

using FuzzInput = std::vector<std::pair<unsigned char, unsigned char>>;

void
fuzzTest(const FuzzInput& input)
{
  aoc22::day2::Guide realInput{};
  realInput.reserve(input.size());
  for (const auto& m : input) {
    const auto m1 = static_cast<aoc22::day2::Move>(m.first);
    const auto m2 = static_cast<aoc22::day2::Move>(m.second);
    realInput.emplace_back(std::make_pair(m1, m2));
  }
  try {
    aoc22::day2::solve(realInput);
  } catch (std::runtime_error&) {
  }
}

FUZZ_TEST(Day2FuzzTest, fuzzTest);

#endif
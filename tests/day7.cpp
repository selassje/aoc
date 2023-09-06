
#include "day7.hpp"
#include "inputs.hpp"

#include <catch2/catch.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace {

    using aoc22::day7::Input;

    Input readInput(std::string_view) {
        Input input{};
        return input;
    }
    
};

TEST_CASE("Day7 Example", "[Day7]")
{
  const auto input = readInput(inputs::day7::EXAMPLE);
  const auto& [part_1, part_2] =
    aoc22::day7::solve(input);
  REQUIRE(part_1 == 95437);
}
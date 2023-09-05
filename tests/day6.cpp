#include "day6.hpp"
#include "inputs.hpp"

#include <catch2/catch.hpp>


TEST_CASE("Day6 Example_1", "[Day6]")
{
  const auto& [part_1, part_2] = aoc22::day6::solve("mjqjpqmgbljsphdztnvjfqwrcgsmlb");
  REQUIRE(part_1 == 7);
}

TEST_CASE("Day6 Example_2", "[Day6]")
{
  const auto& [part_1, part_2] = aoc22::day6::solve("bvwbjplbgvbhsrlpgdmjqwftvncz");
  REQUIRE(part_1 == 5);
}
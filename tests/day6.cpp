#include "day6.hpp"
#include "inputs.hpp"

#include <catch2/catch.hpp>
#include <fstream>

namespace {

std::string
readInput(std::string_view path)
{
  std::ifstream ifs{ path.data() };
  std::string line{};
  std::getline(ifs, line);
  return line;
}

}

TEST_CASE("Day6 Example_1", "[Day6]")
{
  const auto& [part_1, part_2] =
    aoc22::day6::solve("mjqjpqmgbljsphdztnvjfqwrcgsmlb");
  REQUIRE(part_1 == 7);
  REQUIRE(part_2 == 19);
}

TEST_CASE("Day6 Example_2", "[Day6]")
{
  const auto& [part_1, part_2] =
    aoc22::day6::solve("bvwbjplbgvbhsrlpgdmjqwftvncz");
  REQUIRE(part_1 == 5);
  REQUIRE(part_2 == 23);
}

TEST_CASE("Day6 Example_3", "[Day6]")
{
  const auto& [part_1, part_2] =
    aoc22::day6::solve("nppdvjthqldpwncqszvftbrmjlhg");
  REQUIRE(part_1 == 6);
  REQUIRE(part_2 == 23);
}

TEST_CASE("Day6 Example_4", "[Day6]")
{
  const auto& [part_1, part_2] =
    aoc22::day6::solve("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg");
  REQUIRE(part_1 == 10);
  REQUIRE(part_2 == 29);

}

TEST_CASE("Day6 Example_5", "[Day6]")
{
  const auto& [part_1, part_2] =
    aoc22::day6::solve("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw");
  REQUIRE(part_1 == 11);
  REQUIRE(part_2 == 26);
}

TEST_CASE("Day6 Input", "[Day6]")
{
  const auto& [part_1, part_2] =
    aoc22::day6::solve(readInput(inputs::day6::INPUT));
  REQUIRE(part_1 == 1848);
  REQUIRE(part_2 == 2308);
}
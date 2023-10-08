
#include "day12.hpp"
#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <fstream>
#include <iterator>
#include <sstream>
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
    std::vector<std::byte> row(line.size(), std::byte{0});
    for (IndexType x = 0; x < line.size(); ++x) {
      auto c = line[x];
      if (c == 'S') {
        input.startPosition = { x, y };
        c = 's';
      }
      if (c == 'E') {
        input.finalPosition = { x, y };
        c = 'e';
      }
      row[x] = static_cast<std::byte>(c - 'a');
    }
    input.grid.push_back(row);
  }
  return input;
}

TEST_CASE("Day12 Example", "[Day12]")
{
  const auto input = readInput(inputs::day12::EXAMPLE);
  const auto& [part_1, part_2] = aoc22::day12::solve(input);
  REQUIRE(part_1 == 31);
  REQUIRE(part_2 == 31);
}
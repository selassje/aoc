
#include "day7.hpp"
#include "inputs.hpp"

#include <catch2/catch.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace {

using aoc22::day7::Input;
using aoc22::day7::Record;

Input
readInput(const std::string_view path)
{
  Input input{};

  std::vector<std::string> words{};
  auto split = [&words](auto& iss) {
    words.clear();
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(),
              std::back_inserter(words));
  };

  std::ifstream ifs{ path.data() };
  std::string line{};
  const std::string command_prefix = "$";
  const std::string ls_command = "ls";
  const std::string cd_command = "cd";
  const std::string directory_prefix = "dir";

  while (std::getline(ifs, line)) {
    Record record{};
    std::istringstream iss{ line };
    split(iss);
    if (words[0] == command_prefix) {
      if (words[1] == ls_command) {
        record = aoc22::day7::List{};
      } else if (words[1] == cd_command) {
        record = aoc22::day7::ChangeDirectory{ .directory = words[2] };
      } else {
        std::abort();
      }
    } else if (words[0] == directory_prefix) {
      record = aoc22::day7::Directory{ .name = words[1] };
    } else {
      record =
        aoc22::day7::File{ .name = words[1], .size = std::stoull(words[0]) };
    }
    input.push_back(record);
  }
  return input;
}

}

TEST_CASE("Day7 Example", "[Day7]")
{
  const auto input = readInput(inputs::day7::EXAMPLE);
  const auto& [part_1, part_2] = aoc22::day7::solve(input);
  REQUIRE(part_1 == 95437);
  REQUIRE(part_2 == 24933642);
}

TEST_CASE("Day7 Input", "[Day7]")
{
  const auto input = readInput(inputs::day7::INPUT);
  const auto& [part_1, part_2] = aoc22::day7::solve(input);
  REQUIRE(part_1 == 1086293);
  REQUIRE(part_2 == 366028);
}

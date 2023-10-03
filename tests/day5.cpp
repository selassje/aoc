
#include "day5.hpp"
#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace {

using aoc22::day5::Input;
using aoc22::day5::Move;
using aoc22::day5::Moves;
using aoc22::day5::Stack;
using aoc22::day5::Stacks;

using Deque = std::deque<char>;
using Deques = std::vector<Deque>;

Input
readInput(const std::string_view path)
{
  std::ifstream ifs{ path.data() };
  std::string line{};
  std::getline(ifs, line);

  const auto stacksNum = 1 + (line.size() - 3) / 4;
  Deques deques(stacksNum);
  Moves moves{};

  ifs.clear();
  ifs.seekg(0);

  auto eraseSubstring = [&line](std::string_view substring) {
    auto ind = line.find(substring);
    line.erase(ind, substring.length());
  };

  while (std::getline(ifs, line)) {
    if (!line.empty()) {
      if (line.starts_with("move")) {
        eraseSubstring("move");
        eraseSubstring("from");
        eraseSubstring("to");

        std::istringstream iss{ line };
        Move m{};
        iss >> m.quantity >> m.from >> m.to;
        moves.push_back(m);
      } else if (line.find('[') != std::string::npos) {
        for (std::size_t i = 0; i < stacksNum; ++i) {
          auto crate = line[4 * i + 1];
          if (crate != ' ') {
            deques[i].push_front(crate);
          }
        }
      }
    }
  }

  Stacks stacks{};
  for (auto& stack : deques) {
    stacks.emplace_back(std::move(stack));
  }
  return { stacks, moves };
}

TEST_CASE("Day5 Example", "[Day5]")
{
  const auto input = readInput(inputs::day5::EXAMPLE);
  const auto& [part_1, part_2] = aoc22::day5::solve(input);
  REQUIRE(part_1 == "CMZ");
  REQUIRE(part_2 == "MCD");
}

TEST_CASE("Day5 Input", "[Day5]")
{
  const auto input = readInput(inputs::day5::INPUT);
  const auto& [part_1, part_2] = aoc22::day5::solve(input);
  REQUIRE(part_1 == "TWSGQHNHL");
  REQUIRE(part_2 == "JNRSCDWPP");
}

}
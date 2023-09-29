
#include "day11.hpp"
#include "inputs.hpp"
#include "tests_main.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

using aoc22::day11::Input;
using aoc22::day11::Monkey;

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

  auto toULL =
    [end = static_cast<char*>(nullptr)](const std::string& word) mutable {
      return std::strtoull(word.c_str(), &end, 10);
    };

  Monkey monkey{};
  while (std::getline(ifs, line)) {
    std::istringstream iss{ line };
    split(iss);

    if (line == "") {
      input.push_back(monkey);
      monkey.worryLevels.clear();
    } else if (words[0] == "Starting") {
      for (std::size_t i = 2; i < words.size(); ++i) {
        monkey.worryLevels.push_back(toULL(words[i]));
      }

    } else if (words[0] == "Operation:") {
      auto getOperand =
        [&toULL](const std::string& word) -> aoc22::day11::Operand {
        if (word == "old") {
          return aoc22::day11::Old{};
        }
        return aoc22::day11::Literal{ toULL(word) };
      };

      monkey.operation.op1 = getOperand(words[3]);
      monkey.operation.op2 = getOperand(words[5]);

      monkey.operation.type = words[4] == "+"
                                ? aoc22::day11::OperationType::Add
                                : aoc22::day11::OperationType::Multiply;

    } else if (words[0] == "Test:") {
      monkey.divisionTest = toULL(words.back());
    } else if (words[0] == "If" && words[1] == "true:") {
      monkey.nextMonkeyTestPass = toULL(words.back());
    } else if (words[0] == "If" && words[1] == "false:") {
      monkey.nextMonkeyTestFail = toULL(words.back());
    }
  }
  input.push_back(monkey);
  return input;
}

TEST_CASE("Day11 Example", "[Day11]")
{
  const auto input = readInput(inputs::day11::EXAMPLE);
  const auto& [part_1, part_2] = aoc22::day11::solve(input);
  REQUIRE(part_1 == 10605);
  REQUIRE(part_2 == 10605);
}

TEST_CASE("Day11 Input", "[Day11]")
{
  const auto input = readInput(inputs::day11::INPUT);
  const auto& [part_1, part_2] = aoc22::day11::solve(input);
  REQUIRE(part_1 == 61005);
  REQUIRE(part_2 == 61005);
}
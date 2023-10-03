
#include "day11.hpp"
#include "inputs.hpp"

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

using aoc22::day11::Input;
using aoc22::day11::Monkey;
using aoc22::day11::Operand;
using enum aoc22::day11::OperationType;

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
      static constexpr auto base = 10;
      return std::strtoull(word.c_str(), &end, base);
    };

  Monkey monkey{};
  while (std::getline(ifs, line)) {
    std::istringstream iss{ line };
    split(iss);

    if (line.empty()) {
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
      monkey.operation.op2 = getOperand(words[5]); // NOLINT

      monkey.operation.type = words[4] == "+" ? Add : Multiply;

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
  REQUIRE(part_2 == 2713310158);
}

TEST_CASE("Day11 Input", "[Day11]")
{
  const auto input = readInput(inputs::day11::INPUT);
  const auto& [part_1, part_2] = aoc22::day11::solve(input);
  REQUIRE(part_1 == 61005);
  REQUIRE(part_2 == 20567144694);
}

#ifdef ENABLE_FUZZ_TESTS
#include <fuzztest/fuzztest.h>

using aoc22::day11::Operand;
using aoc22::day11::Operation;
using aoc22::day11::OperationType;

struct alignas(64) OperationFuzzTest
{
  int type;
  Operand op1;
  Operand op2;
};
struct alignas(128) MonkeyFuzzTest
{
  OperationFuzzTest operation;
  std::uint64_t divisionTest;
  std::size_t nextMonkeyTestPass;
  std::size_t nextMonkeyTestFail;
  std::vector<std::uint64_t> worryLevels;
};

using InputFuzzTest = std::vector<MonkeyFuzzTest>;

void
fuzzTest(const InputFuzzTest& inputFuzzTest)
{
  Input input;
  for (const auto& monkeyFuzzTest : inputFuzzTest) {
    Monkey monkey;
    Operation operation;
    operation.op1 = monkeyFuzzTest.operation.op1;
    operation.op2 = monkeyFuzzTest.operation.op2;
    operation.type = static_cast<OperationType>(monkeyFuzzTest.operation.type);

    monkey.operation = operation;
    monkey.divisionTest = monkeyFuzzTest.divisionTest;
    monkey.nextMonkeyTestFail = monkeyFuzzTest.nextMonkeyTestFail;
    monkey.nextMonkeyTestPass = monkeyFuzzTest.nextMonkeyTestPass;
    monkey.worryLevels = monkeyFuzzTest.worryLevels;

    input.push_back(monkey);
  }
  try {
    aoc22::day11::solve(input);
  } catch (...) { //NOLINT
  }
}

FUZZ_TEST(Day11FuzzTest, fuzzTest);

#endif
#include "day11.hpp"

#include <algorithm>
#include <array>
#include <cstdio>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace aoc22::day11 {

template<int ROUNDS, int DIVIDE_NEW_LEVEL>
std::uint64_t
solveInternal(const Input& input)
{
  if (input.size() < 2) {
    throw std::runtime_error(
      "Aoc22::day11: At least two Monkeys are required in the input");
  }
  {
    std::array<char, 256> messageBuf{};
    for (std::size_t i = 0; i < input.size(); ++i) {
      if (input[i].nextMonkeyTestFail >= input.size()) {
        (void)snprintf( // NOLINT
          messageBuf.data(),
          messageBuf.max_size(),
          "Aoc22::day11: nextMonkeyTestFail of monkey %zu is bigger(%zu) "
          "than the total number of monkeys(%zu)",
          i,
          input[i].nextMonkeyTestFail,
          input.size());
        throw std::runtime_error(messageBuf.data());
      }
      if (input[i].nextMonkeyTestPass >= input.size()) {
        (void)snprintf( // NOLINT
          messageBuf.data(),
          messageBuf.max_size(),
          "Aoc22::day11: nextMonkeyTestPass of monkey %zu is bigger(%zu) "
          "than the total number of monkeys(%zu)",
          i,
          input[i].nextMonkeyTestPass,
          input.size());
        throw std::runtime_error(messageBuf.data());
      }

      if (input[i].divisionTest == 0) {
        (void)snprintf( // NOLINT
          messageBuf.data(),
          messageBuf.max_size(),
          "Aoc22::day11: divisionTest field of monkey %zu is 0",
          i);
        throw std::runtime_error(messageBuf.data());
      }
    }
  }
  std::vector<std::vector<std::uint64_t>> worryLevels{};

  const auto modulo =
    std::accumulate(input.begin(),
                    input.end(),
                    1ULL,
                    [](const auto& value, const auto& monkey) {
                      return value * monkey.divisionTest;
                    });

  for (const auto& m : input) {
    worryLevels.push_back(m.worryLevels);
  }

  auto getOperandValue = [](const std::uint64_t worryLevel,
                            const Operand operand) {
    if (operand.index() == 0) {
      return std::get<Literal>(operand).value;
    }
    return worryLevel;
  };

  auto getNewLevel = [&](const std::uint64_t worryLevel,
                         const Operation& operation) {
    const auto operandValue1 = getOperandValue(worryLevel, operation.op1);
    const auto operandValue2 = getOperandValue(worryLevel, operation.op2);

    if (operation.type == OperationType::Add) {
      return (operandValue1 + operandValue2);
    }
    return (operandValue1 * operandValue2);
  };

  std::vector<std::size_t> inspectionsCount(input.size(), 0);

  for (int round = 0; round < ROUNDS; ++round) {
    for (std::size_t i = 0; i < input.size(); ++i) {
      const auto& monkey = input[i];
      for (const auto worryLevel : worryLevels[i]) {
        const auto newLevel =
          getNewLevel(worryLevel, monkey.operation) % modulo / DIVIDE_NEW_LEVEL;
        const auto nextMonkey = newLevel % monkey.divisionTest == 0
                                  ? monkey.nextMonkeyTestPass
                                  : monkey.nextMonkeyTestFail;

        worryLevels[nextMonkey].push_back(newLevel);
      }
      inspectionsCount[i] += worryLevels[i].size();
      worryLevels[i].clear();
    }
  }

  std::ranges::sort(inspectionsCount, std::ranges::greater{});

  return inspectionsCount[0] * inspectionsCount[1];
}

Result
solve(const Input& input)
{
  return std::make_pair(solveInternal<20, 3>(input),     // NOLINT
                        solveInternal<10000, 1>(input)); // NOLINT
}
};

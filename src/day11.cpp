#include "day11.hpp"

#include <algorithm>
#include <vector>

namespace aoc22::day11 {

Result
solve(const Input& input)
{
  std::vector<std::vector<std::uint64_t>> worryLevels{};
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
      return operandValue1 + operandValue2;
    }
    return operandValue1 * operandValue2;
  };

  std::vector<std::size_t> inspectionsCount(input.size(), 0);

  for (int round = 0; round < 20; ++round) {
    for (std::size_t i = 0; i < input.size(); ++i) {
      const auto& monkey = input[i];
      for (const auto worryLevel : worryLevels[i]) {
        const auto newLevel = getNewLevel(worryLevel, monkey.operation) / 3;
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

  const auto resultPart1 = inspectionsCount[0] * inspectionsCount[1];

  return std::make_pair(resultPart1, resultPart1);
}
};

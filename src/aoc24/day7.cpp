#include "aoc24/day7.hpp"
#include <cstddef>
#include <vector>

namespace aoc24::day7 {

namespace {

using Operands = std::vector<std::size_t>;

bool
isEquationPossibleRec(std::size_t testValue,
                      std::size_t i,
                      const Operands& operands)
{

  if (i == operands.size() - 1) {
    return testValue == operands[i] || testValue / operands[i] == 1;
  }

  const bool add =
    isEquationPossibleRec(testValue - operands[i], i + 1, operands);
  const bool multiply =
    isEquationPossibleRec(testValue / operands[i], i + 1, operands);
  return add || multiply;
}

bool
isEquationPossible(const Record& record)
{
  return isEquationPossibleRec(record.testValue, 1, record.operands);
}

}

Result
solve(const Input& input)
{
  std::size_t part1 = 0;
  for (const auto& record : input) {
    if (isEquationPossible(record)) {
      part1 += record.testValue;
    }
  }
  return { part1, part1 };
}

}
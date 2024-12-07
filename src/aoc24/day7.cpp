#include "aoc24/day7.hpp"
#include <algorithm>
#include <cstddef>
#include <limits>
#include <print>
#include <vector>

namespace aoc24::day7 {

namespace {

constexpr std::size_t MAX = std::numeric_limits<std::size_t>::max();
using Operands = std::vector<std::size_t>;

bool
isEquationPossibleRec(std::size_t testValue,
                      std::size_t i,
                      const Operands& operands)
{
  bool add = true;

  if (i == operands.size() - 2) {
    std::size_t addV = 0;
    if( MAX - operands[i] < operands[i+ 1]) {
      addV = operands[i] + operands[i + 1];
      add = true;
    }
    addV = operands[i] + operands[i + 1];
    const auto multiplyV = operands[i] * operands[i + 1];

    return ( add && testValue == addV) || testValue == multiplyV;
  }
  
  if (testValue > operands[i]) {
    add = isEquationPossibleRec(testValue - operands[i], i + 1, operands);
  }
  const bool multiply =
    isEquationPossibleRec(testValue / operands[i], i + 1, operands);
  return add || multiply;
}

bool
isEquationPossible(const Record& record)
{
  return isEquationPossibleRec(record.testValue, 0, record.operands);
}

}

Result
solve(const Input& input)
{


  std::size_t part1 = 0;
  for (const auto& record : input) {
    auto reversedRecord = record;
    std::reverse(reversedRecord.operands.begin(),
                 reversedRecord.operands.end());
    if (isEquationPossible(reversedRecord)) {
      part1 += record.testValue;
    }
  }
  return { part1, part1 };
}

}
#include "aoc24/day7.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>
#include <print>
#include <vector>

namespace aoc24::day7 {

namespace {
// 5512534504936
// 12343796585840
// 18446744073709551615
constexpr std::size_t MAX = std::numeric_limits<std::size_t>::max();
using Operands = std::vector<std::size_t>;

bool
isEquationPossibleRec(std::size_t testValue,
                      std::size_t i,
                      const Operands& operands)
{
  bool add = false;
  bool multiply = false;

  if (i == operands.size() - 2) {
    std::size_t addV = 0;
    std::size_t multiplyV = 0;
    if (MAX - operands[i] > operands[i + 1]) {
      addV = operands[i] + operands[i + 1];
      add = true;
    }
    if (MAX / operands[i] > operands[i + 1]) {
      multiplyV = operands[i] * operands[i + 1];
      multiply = true;
    }
    //    addV = operands[i] + operands[i + 1];
    // const auto multiplyV = operands[i] * operands[i + 1];

    return (add && testValue == addV) || (multiply && testValue == multiplyV);
  }

  if (testValue > operands[i]) {
    add = isEquationPossibleRec(testValue - operands[i], i + 1, operands);
  }
  if (testValue > operands[i] && testValue % operands[i] == 0) {
    multiply = isEquationPossibleRec(testValue / operands[i], i + 1, operands);
  }
  return add || multiply;
}

bool
isEquationPossible(const Record& record)
{
  return isEquationPossibleRec(record.testValue, 0, record.operands);
}

bool
isEquationPossible2(const Record& record)
{
  const std::size_t operatorsCount = record.operands.size() - 1;
  const std::size_t combinationsCount = std::size_t{ 1 } << operatorsCount;
  for (std::size_t combination = 0; combination < combinationsCount;
       ++combination) {
    std::size_t result = 0;
    for (std::size_t i = 0; i < operatorsCount; ++i) {
      const bool isMultiply = (combination & (std::size_t{1} << i)) != 0;
      if( i == 0) {
        if ( isMultiply) {
          result = record.operands[0] * record.operands[1];
        } else {
          result = record.operands[0] + record.operands[1];
        }
      }
      else {
        if ( isMultiply) {
          result = result * record.operands[i + 1];
        } else {
          result = result + record.operands[i + 1];
        }
      }
    }
    if (result == record.testValue) {
      return true;
    }
  }
  return false;
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
    if (isEquationPossible2(record)) {
      part1 += record.testValue;
    }
  }
  return { part1, part1 };
}

}
#include "aoc24/day7.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <limits>
#include <print>
#include <string>
#include <vector>

namespace aoc24::day7 {

namespace {
using Operands = std::vector<std::size_t>;

bool
isEquationPossible2(const Record& record)
{
  const std::size_t operatorsCount = record.operands.size() - 1;
  const std::size_t combinationsCount = std::size_t{ 1 } << operatorsCount;
  for (std::size_t combination = 0; combination < combinationsCount;
       ++combination) {
    std::size_t result = 0;
    for (std::size_t i = 0; i < operatorsCount; ++i) {
      const bool isMultiply = (combination & (std::size_t{ 1 } << i)) != 0;
      if (i == 0) {
        if (isMultiply) {
          result = record.operands[0] * record.operands[1];
        } else {
          result = record.operands[0] + record.operands[1];
        }
      } else {
        if (isMultiply) {
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

std::size_t
concatenate(std::size_t a, std::size_t b)
{
  const auto strA = std::to_string(a);
  const auto strB = std::to_string(b);
  const auto concStr = strA + strB;
  return std::strtoull(concStr.c_str(), nullptr, 10); // NOLINT
}

bool
isEquationPossible3(const Record& record)
{
  const std::size_t operatorsCount = record.operands.size() - 1;
  const auto combinationsCount = static_cast<std::size_t>(
    std::pow(4.0, static_cast<double>(operatorsCount)));

  for (std::size_t combination = 0; combination < combinationsCount;
       ++combination) {
    std::size_t result = 0;
    for (std::size_t i = 0; i < operatorsCount; ++i) {
      const auto opType = (combination & (std::size_t{ 3 } << 2 * i)) >> 2 * i;
      if (i == 0) {
        if (opType == 0) {
          result = record.operands[0] * record.operands[1];
        } else if (opType == 1) {
          result = record.operands[0] + record.operands[1];
        } else if (opType == 2) {
          result = concatenate(record.operands[0], record.operands[1]);
        } else {
          break;
        }
      } else {
        if (opType == 0) {
          result = result * record.operands[i + 1];
        } else if (opType == 1) {
          result = result + record.operands[i + 1];
        } else if (opType == 2) {
          result = concatenate(result, record.operands[i + 1]);
        } else {
          break;
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
  std::size_t part2 = 0;
  for (const auto& record : input) {
    if (isEquationPossible2(record)) {
      part1 += record.testValue;
    }
    if (isEquationPossible3(record)) {
      part2 += record.testValue;
    }
  }
  return { part1, part2 };
}

}
#include "aoc24/day7.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <limits>
#include <print>
#include <string>
#include <vector>

namespace aoc24::day7 {

namespace {
using Operands = std::vector<std::size_t>;

std::size_t
concatenate(std::size_t a, std::size_t b)
{
  const auto strA = std::to_string(a);
  const auto strB = std::to_string(b);
  const auto concStr = strA + strB;
  return std::strtoull(concStr.c_str(), nullptr, 10); // NOLINT
}

template<int N>
bool
isEquationPossible(const Record& record)
{
  const std::size_t operatorsCount = record.operands.size() - 1;
  const std::size_t combinationsCount = std::size_t{ 1 }
                                        << operatorsCount * (N / 2);

  using Operation = std::function<std::size_t(std::size_t, std::size_t)>;
  static std::array<Operation, 3> operations = { std::plus<std::size_t>{},
                                                 std::multiplies<std::size_t>{},
                                                 concatenate };

  for (std::size_t combination = 0; combination < combinationsCount;
       ++combination) {
    std::size_t result = record.operands[0];
    for (std::size_t i = 0; i < operatorsCount; ++i) {
      const auto opType =
        (combination & (std::size_t{ N - 1 } << (N / 2) * i)) >> (N / 2) * i;
      if (opType == 3) {
        goto endComb;
      }
      result = operations[opType](result, record.operands[i + 1]);
    }
    if (result == record.testValue) {
      return true;
    }
    endComb:
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
    if (isEquationPossible<2>(record)) {
      part1 += record.testValue;
    }
    if (isEquationPossible<4>(record)) {
      part2 += record.testValue;
    }
  }
  return { part1, part2 };
}

}
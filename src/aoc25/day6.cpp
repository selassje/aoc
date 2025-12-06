#include "aoc25/day6.hpp"
#include <cstdint>
#include <cmath>
#include <vector>

namespace {
  std::uint64_t performOperation(const aoc25::day6::Problem& problem)
  {
    std::uint64_t result = (problem.operation == aoc25::day6::Operation::Add) ? 0 : 1;
    for (const auto& operand : problem.operands) {
      if (problem.operation == aoc25::day6::Operation::Add) {
        result += operand;
      } else {
        result *= operand;
      }
    }
    return result;
  }
  std::uint64_t performOperation2(const aoc25::day6::Problem& problem)
  {
    const auto operandsCount  = problem.operands.size();
    std::vector<std::string> strOperands{};
    strOperands.resize(operandsCount);
    std::size_t maxDigits = 0;
    for(  std::size_t i = 0; i < operandsCount; ++i) {
      strOperands[i] = std::to_string(problem.operands[i]);
      maxDigits = std::max(maxDigits, strOperands[i].size());
    }

    for(auto & strOperand : strOperands){
      while (strOperand.size() < maxDigits){
        //strOperand = 'x' + strOperand;
        strOperand += 'x';	
      }
    }

    std::uint64_t result = (problem.operation == aoc25::day6::Operation::Add) ? 0 : 1;
    for  (std::size_t digitIndex  = 0 ; digitIndex < maxDigits; ++digitIndex){
      std::string numberStr = "";
      for (std::size_t i = 0; i < operandsCount; ++i) {
         const auto ch = strOperands[i][maxDigits - digitIndex - 1];
         if ( ch != 'x') {
            numberStr += ch;
         }
      }
      const auto number = std::stoull(numberStr);
      if (problem.operation == aoc25::day6::Operation::Add) {
        result += number;
      } else {
        result *= number;
      }
    }
    return result;
  }
}


namespace aoc25::day6 {
Result
solve(const Input& input)
{
  std::uint64_t part1 = 0;
  std::uint64_t part2 = 0;
  for (const auto& problem : input) {
    part1 += performOperation(problem);
    part2 += performOperation2(problem);
  }
  return { part1, part2};
}
}

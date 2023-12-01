#include "aoc23/day1.hpp"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <limits>
#include <map>
#include <ranges>

namespace aoc23::day1 {

using DigitMap = std::map<std::string_view, std::size_t>;

const DigitMap digitMapPart1 = { { "1", 1 }, { "2", 2 }, { "3", 3 },
                                 { "4", 4 }, { "5", 5 }, { "6", 6 },
                                 { "7", 7 }, { "8", 8 }, { "9", 9 } };

const DigitMap digitMapPart2 = {
  { "1", 1 },     { "2", 2 },     { "3", 3 },    { "4", 4 },    { "5", 5 },
  { "6", 6 },     { "7", 7 },     { "8", 8 },    { "9", 9 },    { "one", 1 },
  { "two", 2 },   { "three", 3 }, { "four", 4 }, { "five", 5 }, { "six", 6 },
  { "seven", 7 }, { "eight", 8 }, { "nine", 9 }
};

std::size_t
solve_internal(const Input& input, const DigitMap& digitMap)
{
  std::size_t result = 0;
  for (const auto& line : input) {
    std::size_t minIndexFirst = std::numeric_limits<std::size_t>::max();
    std::size_t maxIndexLast = std::numeric_limits<std::size_t>::max();
    std::size_t firstDigit = 0;
    std::size_t lastDigit = 0;
    for (const auto& [str, digit] : digitMap) {
      const auto firstDigitIndex = line.find(str);
      if (firstDigitIndex != std::string::npos &&
          firstDigitIndex < minIndexFirst) {
        minIndexFirst = firstDigitIndex;
        firstDigit = digit;
      }
      const std::string reversedString(str.rbegin(), str.rend());
      const std::string reversedLine(line.rbegin(), line.rend());
      const auto lastDigitIndex = reversedLine.find(reversedString);
      if (lastDigitIndex != std::string::npos &&
          lastDigitIndex < maxIndexLast) {
        maxIndexLast = lastDigitIndex;
        lastDigit = digit;
      }
    }
    result += firstDigit * 10 + lastDigit;
  }
  return result;
}

Result
solve(const Input& input)
{
  return { solve_internal(input, digitMapPart1),
           solve_internal(input, digitMapPart2) };
}
}

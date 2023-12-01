#include "aoc23/day1.hpp"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <limits>
#include <map>
#include <ranges>

namespace aoc23::day1 {

const std::map<std::string, std::size_t> strToDigitsPart1 = {
  { "0", 0 },   { "1", 1 },     { "2", 2 },     { "3", 3 },    { "4", 4 },
  { "5", 5 },   { "6", 6 },     { "7", 7 },     { "8", 8 },    { "9", 9 }
};

const std::map<std::string_view, std::size_t> strToDigitsPart2 = {
  { "0", 0 },   { "1", 1 },     { "2", 2 },     { "3", 3 },    { "4", 4 },
  { "5", 5 },   { "6", 6 },     { "7", 7 },     { "8", 8 },    { "9", 9 },
  { "one", 1 }, { "two", 2 },   { "three", 3 }, { "four", 4 }, { "five", 5 },
  { "six", 6 }, { "seven", 7 }, { "eight", 8 },
};

Result
solve(const Input& input)
{
  std::size_t part1 = 0;
  //std::size_t part2 = 0;
  for (const auto& line : input) {
      std::size_t minIndexFirst = std::numeric_limits<std::size_t>::max();
      std::size_t minIndexLast = std::numeric_limits<std::size_t>::max();
      std::size_t firstDigit = 0;
      std::size_t lastDigit = 0;
      for( const auto&[str, digit] : strToDigitsPart1 ) {
        const auto firstDigitIndex = line.find(str);
        if (firstDigitIndex != std::string::npos && firstDigitIndex < minIndexFirst) {
          minIndexFirst = firstDigitIndex;
          firstDigit = digit;
        }
        const auto lastDigitIndex = line.find_last_of(str);
        if (lastDigitIndex != std::string::npos && lastDigitIndex < minIndexLast) {
          minIndexLast = lastDigitIndex;
          lastDigit = digit;
        }
      }
      part1 += firstDigit * 10 + lastDigit; 
  }
  return {part1, part1};
}
}

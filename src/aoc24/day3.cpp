#include "aoc24/day3.hpp"
#include <cstddef>
#include <cstdlib>
#include <regex>
#include <string>

namespace aoc24::day3 {
Result
solve(Input input) noexcept // NOLINT(bugprone-exception-escape)
{
  std::size_t part1 = 0;
  std::size_t part2 = 0;
  const std::string strInput{ input };
  const std::regex regex(R"(mul\((\d+),(\d+)\)|do\(\)|don't\(\))");
  const auto endRegexIt = std::sregex_iterator();
  auto toNumber = [](const auto& match) {
    static constexpr int base = 10;
    return std::strtoull(match.str().c_str(), nullptr, base); // NOLINT()
  };
  bool mulEnabled = true;
  auto iter = std::sregex_iterator(strInput.begin(), strInput.end(), regex);
  while (iter != endRegexIt) {
    const auto matchedStr = iter->str();
    if (matchedStr == "do()") {
      mulEnabled = true;
    } else if (matchedStr == "don't()") {
      mulEnabled = false;
    } else {
      const auto left = toNumber((*iter)[1]);
      const auto right = toNumber((*iter)[2]);
      const auto mul = left * right;
      part1 += mul;
      if (mulEnabled) {
        part2 += mul;
      }
    }
    ++iter;
  }
  return { part1, part2 };
}
}
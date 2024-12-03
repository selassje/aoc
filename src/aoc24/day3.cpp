#include "aoc24/day3.hpp"

#include <print>
#include <regex>

namespace aoc24::day3 {
Result
solve(Input input) noexcept
{
  std::size_t part1 = 0;
  const std::string strInput{ input };
  const std::regex regex(R"(mul\((\d+),(\d+)\))");
  const auto endRegexIt = std::sregex_iterator();

  auto toNumber = [](const auto& match) {
    return std::strtoull(match.str().c_str(), nullptr, 10);
  };

  auto iter = std::sregex_iterator(strInput.begin(), strInput.end(), regex);
  while (iter != endRegexIt) {
    const auto left = toNumber((*iter)[1]);
    const auto right = toNumber((*iter)[2]);
    part1 += left * right;
    ++iter;
  }
  return { part1, part1 };
}
}
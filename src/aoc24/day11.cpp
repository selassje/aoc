#include "aoc24/day11.hpp"
#include <cstddef>
#include <cstdlib>
#include <iterator>
#include <string>

namespace aoc24::day11 {

namespace {

auto
blink(std::size_t times, const Input& input)
{
  auto output = input;
  auto split = [](const std::string& strNumber) {
    const auto halfSize = strNumber.size() / 2;
    const auto left = strNumber.substr(0, halfSize);
    const auto right = strNumber.substr(halfSize, halfSize);
    return std::tuple{ left, right };
  };
  auto toNumber = [](const auto& str) {
    return std::strtoull(str.c_str(), nullptr, 10); // NOLINT
  };

  while (times > 0) {
    for (std::size_t i = 0; i < output.size(); ++i) {
      auto& number = output[i];
      if (number == 0) {
        number = 1;
      } else if (const auto strNumber = std::to_string(number);
                 strNumber.size() % 2 == 0) {
        const auto& [left, right] = split(strNumber);
        number = toNumber(right);
        auto it = output.begin();
        std::advance(it, i);
        output.insert(it, toNumber(left));
        ++i;
      } else {
        number *= 2024; // NOLINT
      }
    }
    --times;
  }
  return output.size();
}

}

Result
solve(const Input& input)
{
  const std::size_t part1 = blink(25,input);
  const std::size_t part2 = part1;
  return { part1, part2 };
}

}
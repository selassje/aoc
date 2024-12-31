#include "aoc24/day11.hpp"
#include <cinttypes>
#include <compare>
#include <cstddef>
#include <cstdlib>
#include <iterator>
#include <map>
#include <string>

namespace aoc24::day11 {

namespace {
const auto SPLIT = [](const std::string& strNumber) {
  const auto halfSize = strNumber.size() / 2;
  const auto left = strNumber.substr(0, halfSize);
  const auto right = strNumber.substr(halfSize, halfSize);
  return std::tuple{ left, right };
};
const auto TONUMBER = [](const auto& str) {
  return std::strtoull(str.c_str(), nullptr, 10); // NOLINT
};

struct Key
{
  std::size_t number;
  std::size_t iteration;
  std::strong_ordering operator<=>(const Key&) const = default;
};

std::size_t
getCount(std::size_t number, std::size_t iteration)
{
  static std::map<Key, std::size_t> cache{};
  const auto key = Key{ number, iteration };
  if (cache.contains(key)) {
    return cache[key];
  }
  std::size_t count = 0;

  if (iteration == 0) {
    count = 1;
  } else {
    if (number == 0) {
      count = getCount(1, iteration - 1);
    } else {
      const auto strNumber = std::to_string(number);
      if (strNumber.size() % 2 == 0) {
        const auto& [left, right] = SPLIT(strNumber);
        count = getCount(TONUMBER(left), iteration - 1) +
                getCount(TONUMBER(right), iteration - 1);
      } else {
        count = getCount(number * 2024, iteration - 1); // NOLINT
      }
    }
  }

  cache[key] = count;
  return count;
}
auto
blink2(std::size_t times, const Input& input)
{
  std::size_t count = 0;
  for (const auto number : input) {
    count += getCount(number, times);
  }
  return count;
}
/*
auto
blink(std::size_t times, const Input& input)
{
  auto output = input;

  while (times > 0) {
    for (std::size_t i = 0; i < output.size(); ++i) {
      auto& number = output[i];
      if (number == 0) {
        number = 1;
      } else if (const auto strNumber = std::to_string(number);
                 strNumber.size() % 2 == 0) {
        const auto& [left, right] = SPLIT(strNumber);
        number = TONUMBER(right);
        auto it = output.begin();
        std::advance(it, i);
        output.insert(it, TONUMBER(left));
        ++i;
      } else {
        number *= 2024; // NOLINT
      }
    }
    --times;
  }
  return output.size();
}
*/
}

Result
solve(const Input& input)
{
  const std::size_t part1 = blink2(25, input);
  const std::size_t part2 = blink2(75, input);
  return { part1, part2 };
}

}
#include "aoc24/day11.hpp"
#include <algorithm> //NOLINT
#include <compare>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <map>
#include <ranges>
#include <string>
#include <tuple>

namespace aoc24::day11 {

namespace {

auto
split(const std::string& strNumber)
{
  const auto halfSize = strNumber.size() / 2;
  const auto left = strNumber.substr(0, halfSize);
  const auto right = strNumber.substr(halfSize, halfSize);
  return std::tuple{ left, right };
};

auto
toNumber(const auto& str)
{
  constexpr static int base = 10;
  return std::strtoull(str.c_str(), nullptr, base);
};

struct Key
{
  std::size_t number;
  std::size_t blinks;
  std::strong_ordering operator<=>(const Key&) const = default;
};

std::size_t
getCount(std::size_t number, std::size_t blinks)
{
  if (blinks == 0) {
    return 1;
  }

  static std::map<Key, std::size_t> cache{}; // NOLINT
  const auto key = Key{ number, blinks };
  if (cache.contains(key)) {
    return cache[key];
  }

  std::size_t& count = cache[key];

  auto getCountNext = [&blinks](const std::size_t n) {
    return getCount(n, blinks - 1);
  };

  if (number == 0) {
    count = getCountNext(1);
  } else if (const auto strNumber = std::to_string(number);
             strNumber.size() % 2 == 0) {
    const auto& [left, right] = split(strNumber);
    count = getCountNext(toNumber(left)) + getCountNext(toNumber(right));
  } else {
    count = getCountNext(number * 2024); // NOLINT
  }
  return count;
}

template<std::size_t BLINKS>
auto
getCount(const Input& input)
{
  namespace rng = std::ranges;
  auto counts = input | rng::views::transform([](const auto n) {
                  return getCount(n, BLINKS);
                });      // NOLINT
  return rng::fold_left( // NOLINT
    counts,
    0ULL,
    std::plus{});
}
}

Result
solve(const Input& input)
{
  const std::size_t part1 = getCount<25>(input);
  const std::size_t part2 = getCount<75>(input);
  return { part1, part2 };
}
}
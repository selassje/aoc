#ifndef AOC22_DAY1_HPP
#define AOC22_DAY1_HPP

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnull-dereference"
#endif

#include <algorithm>

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif

#include <concepts>
#include <numeric>
#include <utility>
#include <vector>

namespace aoc22::day1 {

template<typename T>
concept nested_unsigned_range = std::ranges::range<T> && requires(T& t) {
  { std::ranges::range_value_t<T>{} } -> std::ranges::range;
  {
    std::ranges::range_value_t<std::ranges::range_value_t<T>>{}
  } -> std::unsigned_integral;
};

auto
getTopThreeCaloriesPerElf(const nested_unsigned_range auto& input)
{
  std::vector<unsigned int> caloriesPefElf{};
  for (const auto& inner : input) {
    const unsigned int currentElfCalories =
      std::reduce(inner.begin(), inner.end(), 0U);
    caloriesPefElf.push_back(currentElfCalories);
  }
  std::ranges::partial_sort(caloriesPefElf,
                            caloriesPefElf.begin() + 3,
                            [](auto a, auto b) { return a > b; });
  return std::vector<unsigned int>{ caloriesPefElf.begin(),
                                    caloriesPefElf.begin() + 3 };
}

auto
solve(const nested_unsigned_range auto& input)
{
  const auto topThree = getTopThreeCaloriesPerElf(input);
  const auto sum = std::reduce(topThree.begin(), topThree.end(), 0U);
  const auto max = topThree[0];
  return std::make_pair(max, sum);
}

}

#endif
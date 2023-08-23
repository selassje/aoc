#ifndef AOC22_DAY1_HPP
#define AOC22_DAY1_HPP

#include <algorithm>
#include <concepts>
#include <numeric>
#include <utility>
#include <vector>

namespace aoc22::day1 {

template<typename T>
concept nested_unsigned_range = std::ranges::range<T>&& requires(T& t)
{
  {
    std::ranges::range_value_t<T> {}
  }
  ->std::ranges::range;
  {
    std::ranges::range_value_t<std::ranges::range_value_t<T>> {}
  }
  ->std::unsigned_integral;
};

auto
get_top_three_calories_per_elf(const nested_unsigned_range auto& input)
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
  const auto top_three = get_top_three_calories_per_elf(input);
  const auto sum = std::reduce(top_three.begin(), top_three.end(), 0U);
  const auto max = top_three[0];
  return std::make_pair(max, sum);
}

}

#endif
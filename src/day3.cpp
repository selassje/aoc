#include <algorithm>
#include <cassert>
#include <cctype>
#include <day3.hpp>

namespace aoc22::day3 {
unsigned int
solve(const std::vector<std::string>& input) noexcept
{
  unsigned int result = 0;

  auto get_priority = [](char c) {
    const char base = static_cast<bool>(std::islower(c)) ? 'a' : 'A' - 26;
    return 1 + c - base;
  };

  for (const auto& rucksack : input) {
    assert(rucksack.size() % 2 == 0 && rucksack.size() >= 2);

    const auto middle = [&rucksack]() {
      auto middle = rucksack.begin();
      std::advance(middle, rucksack.size() / static_cast<std::size_t>(2));
      return middle;
    }();

    const auto common_type =
      std::find_first_of(rucksack.begin(), middle, middle, rucksack.end());

    assert(common_type != rucksack.end());

    result += get_priority(*common_type);
  }
  return result;
}
}; // namespace aoc22::day3

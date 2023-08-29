#include <algorithm>
#include <cassert>
#include <cctype>
#include <day3.hpp>

namespace aoc22::day3 {

std::string
get_common_types(const std::string& items_1, const std::string& items_2)
{
  std::string common_types{};
  auto next_common_type = items_1.begin();
  auto find_next_common_type = [&]() {
    return std::find_first_of(
      next_common_type, items_1.end(), items_2.begin(), items_2.end());
  };

  next_common_type = find_next_common_type();
  while (next_common_type != items_1.end()) {
    common_types.push_back(*next_common_type);
    std::advance(next_common_type, 1);
    next_common_type = find_next_common_type();
  }

  return common_types;
}

char
get_common_type(const std::string& items_1, const std::string& items_2)
{
  return get_common_types(items_1, items_2)[0];
}

std::pair<unsigned int, unsigned int>
solve(const std::vector<std::string>& input)
{
  unsigned int result_part1 = 0;
  unsigned int result_part2 = 0;

  auto get_priority = [](char c) {
    const char base = static_cast<bool>(std::islower(c)) ? 'a' : 'A' - 26;
    return 1 + c - base;
  };

  for (std::size_t i = 0; i < input.size(); ++i) {
    const auto& rucksack = input[i];
    assert(rucksack.size() % 2 == 0 && rucksack.size() >= 2);

    const auto middle = [&rucksack]() {
      auto middle = rucksack.begin();
      std::advance(middle, rucksack.size() / static_cast<std::size_t>(2));
      return middle;
    }();

    const auto common_type =
      get_common_type({ rucksack.begin(), middle }, { middle, rucksack.end() });

    result_part1 += get_priority(common_type);

    if ((i + 1) % 3 == 0) {
      const auto& first_rucksack = input[i - 2];
      const auto& second_rucksack = input[i - 1];

      const auto common_types =
        get_common_types(first_rucksack, second_rucksack);

      const auto common_type_group = get_common_type(common_types, rucksack);
      result_part2 += get_priority(common_type_group);
    }
  }
  return std::make_pair(result_part1, result_part2);
}
}; // namespace aoc22::day3

#include <algorithm>
#include <cassert>
#include <cctype>
#include <day3.hpp>

namespace aoc22::day3 {
std::pair<unsigned int, unsigned int>
solve(const std::vector<std::string>& input) noexcept
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
      std::find_first_of(rucksack.begin(), middle, middle, rucksack.end());

    assert(common_type != rucksack.end());

    result_part1 += get_priority(*common_type);

    std::vector<char> common_types{};
    if ((i + 1) % 3 == 0) {
      const auto& first_rucksack = input[i - 2];
      const auto& second_rucksack = input[i - 1];
      auto common_type = first_rucksack.begin();
      auto find_first_common = [&]() {
        return std::find_first_of(common_type,
                                  first_rucksack.end(),
                                  second_rucksack.begin(),
                                  second_rucksack.end());
      };

      common_type = find_first_common();
      while (common_type != first_rucksack.end()) {
        common_types.push_back(*common_type);
        std::advance(common_type, 1);
        find_first_common();
      }
      const auto common_type_2 = std::find_first_of(common_types.begin(),
                                                    common_types.end(),
                                                    rucksack.begin(),
                                                    rucksack.end());
      result_part2 += get_priority(*common_type_2);
      common_types.clear();
    }
  }
  return std::make_pair(result_part1, result_part2);
}
}; // namespace aoc22::day3

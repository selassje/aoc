#include "aoc22/day3.hpp"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

namespace aoc22::day3 {
namespace {
std::string
getCommonTypes(const std::string& items_1, const std::string& items_2)
{
  std::string commonTypes{};
  auto nextCommonType = items_1.begin();
  auto findNextCommonType = [&]() {
    return std::find_first_of(
      nextCommonType, items_1.end(), items_2.begin(), items_2.end());
  };

  nextCommonType = findNextCommonType();
  while (nextCommonType != items_1.end()) {
    commonTypes.push_back(*nextCommonType);
    std::advance(nextCommonType, 1);
    nextCommonType = findNextCommonType();
  }

  return commonTypes;
}

char
getCommonType(const std::string& items_1, const std::string& items_2)
{
  return getCommonTypes(items_1, items_2)[0];
}
}
std::pair<unsigned int, unsigned int>
solve(const std::vector<std::string>& input)
{
  unsigned int resultPart1 = 0;
  unsigned int resultPart2 = 0;

  auto getPriority = [](char c) {
    const char base = static_cast<bool>(std::islower(c)) ? 'a' : 'A' - 26;
    return static_cast<unsigned int>(1 + c - base);
  };

  for (std::size_t i = 0; i < input.size(); ++i) {
    const auto& rucksack = input[i];
    assert(rucksack.size() % 2 == 0 && rucksack.size() >= 2);

    const auto getMiddle = [&rucksack]() {
      auto middle = rucksack.begin();
      std::advance(middle, rucksack.size() / static_cast<std::size_t>(2));
      return middle;
    }();

    const auto commonType = getCommonType({ rucksack.begin(), getMiddle },
                                          { getMiddle, rucksack.end() });

    resultPart1 += getPriority(commonType);

    if ((i + 1) % 3 == 0) {
      const auto& firstRucksack = input[i - 2];
      const auto& secondRucksack = input[i - 1];

      const auto commonTypes = getCommonTypes(firstRucksack, secondRucksack);

      const auto commonTypeGroup = getCommonType(commonTypes, rucksack);
      resultPart2 += getPriority(commonTypeGroup);
    }
  }
  return std::make_pair(resultPart1, resultPart2);
}
}; // namespace aoc22::day3

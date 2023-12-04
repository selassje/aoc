#include "aoc23/day4.hpp"

#include <algorithm>
#include <cstddef>
#include <vector>

namespace aoc23::day4 {

std::size_t
getCardMatches(const Card& card)
{
  std::size_t result = 0;
  for (const auto& winningNumber : card.winningNumbers) {
    if (std::ranges::find(card.actualNumbers, winningNumber) !=
        card.actualNumbers.end()) {
      ++result;
    }
  }
  return result;
}

Result
solve(const Input& input)
{
  const auto originalCardCount = input.size();
  std::vector<std::size_t> cardCount(originalCardCount, 1);

  std::size_t part1 = 0;
  for (std::size_t i = 0; i < originalCardCount; ++i) {
    const auto& card = input[i];
    const auto matches = getCardMatches(card);
    part1 += matches == 0 ? 0 : 1ULL << (matches - 1);
    for (std::size_t j = i + 1; j <= i + matches && j < originalCardCount;
         ++j) {
      cardCount[j] += cardCount[i];
    }
  }
  std::size_t part2 = 0;
  for (const auto& count : cardCount) {
    part2 += count;
  }
  return { part1, part2 };
}

}
#include "aoc23/day4.hpp"

#include <algorithm>
#include <cmath>

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
  std::size_t part1 = 0;
  for (const auto& card : input) {
    const auto matches = getCardMatches(card);
    part1 += static_cast<std::size_t>(std::pow(2, matches - 1));
  }
  return { part1, part1 };
}

}
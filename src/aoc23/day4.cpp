#include "aoc23/day4.hpp"

#include <algorithm>

namespace aoc23::day4 {

std::size_t
getCardValue(const Card& card)
{
  std::size_t value = 0;
  for (const auto& winningNumber : card.winningNumbers) {
    if (std::ranges::find(card.actualNumbers, winningNumber) !=
        card.actualNumbers.end()) {
      if (value == 0) {
        value = 1;
      } else {
        value *= 2;
      }
    }
  }
  return value;
}

Result
solve(const Input& input)
{
  std::size_t part1 = 0;
  for (const auto& card : input) {
    part1 += getCardValue(card);
  }
  return { part1, part1 };
}

}
#include "aoc23/day7.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
namespace aoc23::day7 {

enum class HandType : std::uint8_t
{
  FiveOfKind = 6,
  FourOfKind = 5,
  FullHouse = 4,
  ThreeOfKind = 3,
  TwoPairs = 2,
  OnePair = 1,
  HighCard = 0
};

using enum HandType;
using enum Card;
namespace {
HandType
getType(const Hand& hand, bool part2) noexcept
{
  std::array<std::size_t, 14> cardCount{};
  for (const auto& card : hand) {
    ++cardCount[static_cast<std::size_t>(card)];
  }
  const auto jackOrJokerCount = std::ranges::count(hand, Card::JackOrJoker);
  auto count = [&cardCount](const std::size_t i) {
    return std::ranges::count(cardCount, i);
  };
  const auto pairCount = count(2);
  const auto isThree = count(3) == 1;
  const auto isFour = count(4) == 1;
  const auto isFive = count(5) == 1;

  if (part2 && jackOrJokerCount > 0) {
    if (jackOrJokerCount >= 4) {
      return FiveOfKind;
    }
    if (jackOrJokerCount == 3) {
      if (pairCount == 1) {
        return FiveOfKind;
      }
      return FourOfKind;
    }
    if (jackOrJokerCount == 2) {
      if (isThree) {
        return FiveOfKind;
      }
      if (pairCount == 2) {
        return FourOfKind;
      }
      return ThreeOfKind;
    }
    if (jackOrJokerCount == 1) {
      if (isFour) {
        return FiveOfKind;
      }
      if (isThree) {
        return FourOfKind;
      }
      if (pairCount == 2) {
        return FullHouse;
      }
      if (pairCount == 1) {
        return ThreeOfKind;
      }
    }
    return OnePair;
  }
  if (isFive) {
    return FiveOfKind;
  }
  if (isFour) {
    return FourOfKind;
  }
  if (isThree) {
    return pairCount == 1 ? FullHouse : ThreeOfKind;
  }
  if (pairCount == 2) {
    return TwoPairs;
  }
  if (pairCount == 1) {
    return OnePair;
  }
  return HighCard;
}

bool
compareHands(const Hand& handL, const Hand& handR, bool part2)
{
  const auto typeL = static_cast<std::size_t>(getType(handL, part2));
  const auto typeR = static_cast<std::size_t>(getType(handR, part2));
  if (typeL == typeR) {
    for (std::size_t i = 0; i < handL.size(); ++i) {
      auto value = [part2](const Card card) -> std::size_t {
        if (part2 && card == JackOrJoker) {
          return 0;
        }
        return static_cast<std::size_t>(card);
      };
      const auto cardL = value(handL[i]);
      const auto cardR = value(handR[i]);
      if (cardL < cardR) {
        return true;
      }
      if (cardL > cardR) {
        return false;
      }
    }
    return false;
  }
  return typeL < typeR;
}
}
Result
solve(const Input& input)
{
  Input rankedHands = input;
  auto solveInternal = [&rankedHands](const bool part2) {
    std::ranges::sort(rankedHands, [part2](const auto& l, const auto& r) {
      return compareHands(l.hand, r.hand, part2);
    });
    std::size_t result = 0;
    for (std::size_t i = 0; i < rankedHands.size(); ++i) {
      result += (i + 1) * rankedHands[i].bid;
    }
    return result;
  };

  return { solveInternal(false), solveInternal(true) };
}

}
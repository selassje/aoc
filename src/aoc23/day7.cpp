#include "aoc23/day7.hpp"

#include <algorithm>
#include <array>

namespace aoc23::day7 {

enum class HandType
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

HandType
getType(const Hand& hand, bool part2) noexcept
{
  std::array<std::size_t, 14> cardCount{};
  for (const auto& card : hand) {
    ++cardCount[static_cast<std::size_t>(card)];
  }
  const auto endIt = cardCount.end();
  const auto jackOrJokerCount = std::ranges::count(hand, Card::JackOrJoker);
  if (part2 && jackOrJokerCount > 0) {
    if ( jackOrJokerCount >= 4) {
      return FiveOfKind;
    }
    const auto pairCount = std::ranges::count(cardCount, 2);
    if ( jackOrJokerCount == 3) {
        if ( pairCount > 0) {
          return FiveOfKind;
        }
        return FourOfKind;  
    }
    const auto threesCount = std::ranges::count(cardCount, 3);
    if ( jackOrJokerCount == 2) {
        if ( threesCount > 0)
        {
          return FiveOfKind;
        }
        if ( pairCount > 1) {
          return FourOfKind;
        }
        return ThreeOfKind;
    }
    const auto foursCount = std::ranges::count(cardCount, 4);
    if ( jackOrJokerCount == 1) {
        if ( foursCount > 0)
        {
          return FiveOfKind;
        }
        if ( threesCount > 0) {
          return FourOfKind;
        }
        if ( pairCount > 1) {
          return FullHouse;
        }
        if ( pairCount == 1) {
          return ThreeOfKind;
        }
    }
    return OnePair;
  } else {
    auto findIt = std::ranges::find(cardCount, 5);
    if (findIt != endIt) {
      return FiveOfKind;
    }
    findIt = std::ranges::find(cardCount, 4);
    if (findIt != endIt) {
      return FourOfKind;
    }
    const auto findThree = std::ranges::find(cardCount, 3);
    const auto pairCount = std::ranges::count(cardCount, 2);

    if (findThree != endIt) {
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
}

bool
compareHands(const Hand& handL, const Hand& handR, bool part2)
{
  const auto typeL = static_cast<std::size_t>(getType(handL, part2));
  const auto typeR = static_cast<std::size_t>(getType(handR, part2));
  if (typeL == typeR) {
    for (std::size_t i = 0; i < handL.size(); ++i) {
      const auto cardL = static_cast<std::size_t>(handL[i]);
      const auto cardR = static_cast<std::size_t>(handR[i]);
      if (cardL < cardR) {
        return true;
      } else if (cardL > cardR) {
        return false;
      }
    }
    abort();
    return false;
  }
  return typeL < typeR;
}

Result
solve(const Input& input)
{
  Input rankedHands = input;
  std::ranges::sort(rankedHands, [](const auto& l, const auto& r) {
    return compareHands(l.hand, r.hand, false);
  });
  std::size_t part1 = 0;
  for (std::size_t i = 0; i < input.size(); ++i) {
    part1 += (i + 1) * rankedHands[i].bid;
  }
  std::ranges::sort(rankedHands, [](const auto& l, const auto& r) {
    return compareHands(l.hand, r.hand, true);
  });
  std::size_t part2 = 0;
  for (std::size_t i = 0; i < input.size(); ++i) {
    part2 += (i + 1) * rankedHands[i].bid;
  }
  return { part1, part2 };
}

}
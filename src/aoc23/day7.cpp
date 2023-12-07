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
getType(const Hand& hand) noexcept
{
  std::array<std::size_t, 13> cardCount{};
  for (const auto& card : hand)
  {
    ++cardCount[static_cast<std::size_t>(card)];
  }
  const auto endIt = cardCount.end();
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

bool
compareHands(const Hand& handL, const Hand& handR)
{
  const auto typeL = static_cast<std::size_t>(getType(handL));
  const auto typeR = static_cast<std::size_t>(getType(handR));
  if (typeL == typeR) {
    for (std::size_t i = 0; i < handL.size(); ++i) {
      const auto cardL = static_cast<std::size_t>(handL[i]);
      const auto cardR = static_cast<std::size_t>(handR[i]);
      if (cardL < cardR) {
        return true;
      } else if ( cardL > cardR) {
        return false;
      }
    }
    return false;
  }
  return typeL < typeR;
}

Result
solve(const Input& input)
{
  Input rankedHands = input;
  std::size_t part1= input.size();
  std::ranges::sort(rankedHands, [](const auto &l, const auto &r) {
    return compareHands(l.hand, r.hand);
  });
  part1 = 0;
  for ( std::size_t i = 0 ; i < input.size(); ++i) {
    part1 += (i + 1) * rankedHands[i].bid;
  }
  return { part1, part1 };
}
}
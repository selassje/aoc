#include "day2.hpp"
#include <numeric>
#include <unordered_map>
#include <vector>
namespace aoc22::day2 {
enum class Outcome : unsigned char
{
  Loss,
  Draw = 3,
  Win = 6,
};

namespace {

using enum Move;
using enum Outcome;

constexpr unsigned int
evaluateMove(Move m)
{
  return static_cast<unsigned int>(m);
}

const std::unordered_map<Move, Move> beats{ { Rock, Sciscors }, // NOLINT
                                            { Sciscors, Paper },
                                            { Paper, Rock } };

constexpr unsigned int
evaluateOutcome(Move oponent, Move you)
{

  Outcome outcome{ Draw };

  if (oponent != you) {
    outcome = beats.at(you) == oponent ? Win : Loss;
  }
  return static_cast<unsigned int>(outcome);
}

Move
nextMove(Move oponent, Outcome desiredOutcome)
{
  const Move loosingMove = beats.at(oponent);

  switch (desiredOutcome) {
    case Draw:
      return oponent;
    case Loss:
      return loosingMove;
    case Win:
      for (auto m : { Rock, Paper, Sciscors }) {
        if (m != oponent && m != loosingMove) {
          return m;
        }
      }
    default:
      throw 0;
  }
}
}
std::pair<unsigned int, unsigned int>
solve(const Guide& guide)
{
  auto evaluateRound = [](Move oponent, Move you) {
    return evaluateMove(you) + evaluateOutcome(oponent, you);
  };

  auto converMoveToDesiredOutcome = [](const auto move) {
    switch (move) {
      case Rock:
        return Loss;
      case Paper:
        return Draw;
      case Sciscors:
        return Win;
      default:
        throw 0;
    }
  };

  unsigned int score_part1 = 0;
  unsigned int score_part2 = 0;

  for (const auto& [oponent, you] : guide) {
    score_part1 += evaluateRound(oponent, you);
    score_part2 += evaluateRound(
      oponent, nextMove(oponent, converMoveToDesiredOutcome(you)));
  }

  return std::make_pair(score_part1, score_part2);
}
};
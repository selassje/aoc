#include "day2.hpp"

#include <cstdlib>
#include <numeric>
#include <stdexcept>
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
      [[fallthrough]];
    default:
      throw std::runtime_error{ "Next move: unexpected arguments." };
  }
}

Outcome
convertMoveToDesiredOutcome(const Move move)
{
  switch (move) {
    case Rock:
      return Loss;
    case Paper:
      return Draw;
    case Sciscors:
      return Win;
    default:
      throw std::runtime_error{
        "convertMoveToDesiredOutcome: unexpected argument."
      };
  }
};
};

std::pair<unsigned int, unsigned int>
solve(const Guide& guide)
{
  for (std::size_t i = 0; i < guide.size(); ++i) {
    auto check_move = [](const Move& m) {
      const auto c = static_cast<unsigned char>(m);
      static constexpr auto rock = static_cast<unsigned char>(Rock);
      static constexpr auto scissors = static_cast<unsigned char>(Sciscors);
      return c >= rock && c <= scissors;
    };
    if (!check_move(guide[i].first)) {
      throw std::runtime_error{
        "aoc22::day2::solve first move out of valid range"
      };
    }
    if (!check_move(guide[i].second)) {
      throw std::runtime_error{
        "aoc22::day2::solve second move out of valid range"
      };
    }
  };

  auto evaluateRound = [](Move oponent, Move you) {
    return evaluateMove(you) + evaluateOutcome(oponent, you);
  };

  unsigned int scorePart1 = 0;
  unsigned int scorePart2 = 0;

  for (const auto& [oponent, you] : guide) {
    scorePart1 += evaluateRound(oponent, you);
    scorePart2 += evaluateRound(
      oponent, nextMove(oponent, convertMoveToDesiredOutcome(you)));
  }

  return std::make_pair(scorePart1, scorePart2);
}
};

#ifdef ENABLE_TESTS

#include <catch2/catch_test_macros.hpp>
namespace aoc22::day2 {

TEST_CASE("Day2 nextMove", "[Day2]")
{
  try {
    nextMove(Rock, static_cast<Outcome>(10)); // NOLINT
  } catch (std::runtime_error&) {
  }
}

TEST_CASE("Day2 convertMoveToDesiredOutcome", "[Day2]")
{
  try {
    convertMoveToDesiredOutcome(static_cast<Move>(10)); // NOLINT
  } catch (std::runtime_error&) {
  }
}
};

#endif
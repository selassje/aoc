#include "day2.hpp"

#include <cstddef>
#include <exception>
#include <format>
#include <string>
#include <unordered_map>
#include <utility>

using enum aoc22::day2::Move;

namespace aoc22::day2 {

struct ExceptionBase : std::exception
{
  std::string message;
  [[nodiscard]] const char* what() const noexcept override
  {
    return message.c_str();
  }
  explicit ExceptionBase(std::string message_)
    : message(std::move(message_))
  {
  }
};

struct InputErrorMoveOutOfRange : ExceptionBase
{
  explicit InputErrorMoveOutOfRange(std::size_t index, Move move, bool first)
    : ExceptionBase(
        std::format("Aoc22::day2 {} move of turn {} is out of range({}).",
                    first ? "First" : "Second",
                    index,
                    static_cast<unsigned char>(move)))
  {
  }
};

namespace {

enum class Outcome : unsigned char
{
  Loss = 0,
  Draw = 3,
  Win = 6,
};

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
      throw std::exception{};
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
      throw std::exception{

      };
  }
};
};

std::pair<unsigned int, unsigned int>
solve(const Guide& guide)
{
  for (std::size_t i = 0; i < std::size(guide); ++i) {
    auto const& [first, second] = guide[i];
    auto checkMove = [](const Move& m) {
      const auto c = static_cast<unsigned char>(m);
      static constexpr auto rock = static_cast<unsigned char>(Rock);
      static constexpr auto scissors = static_cast<unsigned char>(Sciscors);
      return c >= rock && c <= scissors;
    };
    if (!checkMove(first)) {
      throw InputErrorMoveOutOfRange{ i, first, true };
    }
    if (!checkMove(second)) {
      throw InputErrorMoveOutOfRange{ i, second, false };
    }
  }

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
#include <iostream>

namespace aoc22::day2 {

TEST_CASE("Day2 nextMove", "[Day2]")
{
  try {
    nextMove(Rock, static_cast<Outcome>(10)); // NOLINT
  } catch (ExceptionBase&) {
    std::cout << "Exception expected\n";
  }
}

TEST_CASE("Day2 convertMoveToDesiredOutcome", "[Day2]")
{
  try {
    convertMoveToDesiredOutcome(static_cast<Move>(10)); // NOLINT
  } catch (ExceptionBase&) {
    std::cout << "Exception expected\n";
  }
}
};

#endif
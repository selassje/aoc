
#ifndef AOC23_DAY9_HPP
#define AOC23_DAY9_HPP

#include <cstdint>

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnull-dereference"
#endif

#include <vector>

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif

namespace aoc23::day9 {

enum class Direction : std::uint8_t
{
  Left,
  Right
};

using Values = std::vector<std::int64_t>;

using Input = std::vector<Values>;

struct Result
{
  std::int64_t part1;
  std::int64_t part2;
};

Result
solve(const Input& input);
}

#endif
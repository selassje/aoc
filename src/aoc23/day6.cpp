#include "aoc23/day6.hpp"

namespace aoc23::day6 {

std::size_t recordBeatingCount(const Race& race) {
  std::size_t result = 0;
  for( std::size_t t = 0 ; t <= race.time; ++t) {
    const auto distance = t * ( race.time -t );
    if (distance > race.recordDistance) {
      ++result;
    }
  }
  return result;
}

Result
solve(const Input& input)
{
  std::size_t part1 = 1;
  for( const auto& race : input) {
    part1 *= recordBeatingCount(race);
  }
  return { part1 , part1 };
}
}
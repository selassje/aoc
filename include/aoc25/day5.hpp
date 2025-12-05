
#ifndef AOC25_DAY5_HPP
#define AOC25_DAY5_HPP

#include <cstdint>
#include <vector>

namespace aoc25::day5 {

struct Range
{
  std::uint64_t from;
  std::uint64_t to;
};

using Ranges = std::vector<Range>;

using IngredientIds = std::vector<std::uint64_t>;

struct Input {
  Ranges freeshIgredients;
  IngredientIds availableIndgredientIds;
};

struct Result
{
  std::uint64_t part1;
  std::uint64_t part2;
};

Result
solve(const Input& input);

}

#endif
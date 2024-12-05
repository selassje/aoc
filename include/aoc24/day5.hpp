
#ifndef AOC24_DAY5_HPP
#define AOC24_DAY5_HPP

#include <utility>
#include <vector>
namespace aoc24::day5 {

using Order = std::pair<std::size_t, std::size_t>;
using Orders = std::vector<Order>;

using Update = std::vector<std::size_t>;
using Updates = std::vector<Update>;

struct Input
{
  Orders orders;
  Updates updates;
};

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

Result
solve(const Input& input);

};
#endif
#ifndef AOC22_DAY8_HPP
#define AOC22_DAY8_HPP

#include <utility>
#include <vector>

namespace aoc22::day8 {

using Grid = std::vector<std::vector<std::byte>>;

std::pair<std::size_t, std::size_t>
solve(const Grid& grid);
};

#endif
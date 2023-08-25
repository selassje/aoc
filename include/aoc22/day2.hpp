
#ifndef AOC22_DAY2_HPP
#define AOC22_DAY2_HPP

#include <vector>
#include <utility>

namespace aoc22::day2 {

enum class Move : unsigned char {
    Rock = 1,
    Paper,
    Sciscors
};

using Guide = std::vector<std::pair<Move, Move>>;

unsigned int solve(const Guide&) noexcept;

}

#endif
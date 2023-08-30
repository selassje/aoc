
#ifndef AOC22_DAY4_HPP
#define AOC22_DAY4_HPP

#include <utility>
#include <vector>

namespace aoc22::day4 {

using Range = std::pair<unsigned int, unsigned int>;
using Result = Range;
using Pair = std::pair<Range, Range>;
using Input = std::vector<Pair>;

Result
solve(const Input& input);

}

#endif
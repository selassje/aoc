#include "day16.hpp"

#include <algorithm>
#include <ranges>
#include <string>

namespace aoc22::day16 {

Valve getValve(std::string_view name, const Input& input) {
  return *std::ranges::find(input, name, &Valve::name);     
}

std::size_t maxPressureRelease(std::string valveName, std::size_t minutes, Input input ) {
  return 0;
}


Result solve(const Input &input) {
    const auto part1 = maxPressureRelease("AA",30, input);
    return {part1, part1};
}

};
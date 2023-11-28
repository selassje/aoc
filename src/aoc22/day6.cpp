#include "aoc22/day6.hpp"

#include <array>
#include <cstddef>
#include <string_view>
#include <unordered_set>
#include <utility>

namespace aoc22::day6 {

namespace {

template<std::size_t marker_length>
std::size_t
solveInternal(std::string_view input)
{
  const std::size_t size = input.size();
  std::array<char, marker_length> lastChars{};

  auto updateLastChars = [&](std::size_t i) {
    const auto start = i - marker_length;
    using DifferenceType = decltype(input)::difference_type;
    std::copy(input.begin() + static_cast<DifferenceType>(start),
              input.begin() +
                static_cast<DifferenceType>(start + marker_length),
              lastChars.begin());
  };

  auto allUnique = [&lastChars]() {
    const std::unordered_set<char> set{ lastChars.begin(), lastChars.end() };
    return set.size() == lastChars.size();
  };

  std::size_t i = marker_length;
  updateLastChars(i);
  while (i < size && !allUnique()) {
    updateLastChars(++i);
  }
  return i;
}

};

std::pair<std::size_t, std::size_t>
solve(std::string_view input)
{
  constexpr auto startMarkerLength = 4;
  constexpr auto messageMarkerLength = 14;
  return std::make_pair(solveInternal<startMarkerLength>(input),
                        solveInternal<messageMarkerLength>(input));
}

};
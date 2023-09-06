#include "day6.hpp"

#include <array>
#include <unordered_set>

namespace aoc22::day6 {

namespace {

template<std::size_t marker_length>
std::size_t
solve_internal(std::string_view input)
{
  const std::size_t size = input.size();
  std::array<char, marker_length> last_chars{};

  auto update_last_chars = [&](std::size_t i) {
    const auto start = i - marker_length;
    using DifferenceType = decltype(input)::difference_type;
    std::copy(input.begin() + static_cast<DifferenceType>(start),
              input.begin() +
                static_cast<DifferenceType>(start + marker_length),
              last_chars.begin());
  };

  auto all_unique = [&last_chars]() {
    const std::unordered_set<char> set{ last_chars.begin(), last_chars.end() };
    return set.size() == last_chars.size();
  };

  std::size_t i = marker_length;
  update_last_chars(i);
  while (i < size && !all_unique()) {
    update_last_chars(++i);
  }
  return i;
}

};

std::pair<std::size_t, std::size_t>
solve(std::string_view input)
{
  constexpr auto start_marker_length = 4;
  constexpr auto message_marker_length = 14;
  return std::make_pair(solve_internal<start_marker_length>(input),
                        solve_internal<message_marker_length>(input));
}

};
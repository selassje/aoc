#include "day6.hpp"

#include <array>
#include <unordered_set>

namespace aoc22::day6 {

std::pair<std::size_t, std::size_t>
solve(std::string_view input)
{
  static const std::size_t size = input.size();
  static constexpr std::size_t marker_length = 4;
  std::array<char, marker_length> last_chars{};

  auto update_last_chars = [&](std::size_t i) {
    const auto start = i - marker_length;
    std::copy(input.begin() + start,
              input.begin() + start + marker_length,
              last_chars.begin());
  };

  auto all_unique = [&last_chars]() {
    std::unordered_set<char> set{ last_chars.begin(), last_chars.end() };
    return set.size() == last_chars.size();
  };

  std::size_t i = marker_length;
  update_last_chars(i);
  while (i < size && !all_unique()) {
    update_last_chars(++i);
  }

  return std::make_pair(i, i);
}

};
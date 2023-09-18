#ifndef AOC22_DAY10_HPP
#define AOC22_DAY10_HPP

#include <array>
#include <cstdint>
#include <utility>
#include <variant>
#include <vector>

namespace aoc22::day10 {
struct Noop
{};

struct AddX
{
  std::int64_t x;
};

using Instruction = std::variant<Noop, AddX>;

using Input = std::vector<Instruction>;

constexpr std::size_t CRT_WIDTH = 40;
constexpr std::size_t CRT_HEIGHT = 6;

using Crt = std::array<std::array<char, CRT_WIDTH>, CRT_HEIGHT>;

using Result = std::pair<std::uint64_t, Crt>;

Result
solve(const Input& input);
};

#endif
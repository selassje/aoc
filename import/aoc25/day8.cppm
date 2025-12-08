export module aoc25.day8;

import std;

export namespace aoc25::day8 {

struct Junction
{
  std::uint64_t x;
  std::uint64_t y;
  std::uint64_t z;

  auto operator<=>(const Junction&) const = default;
};

using Input = std::vector<Junction>;

struct Result
{
  std::uint64_t part1;
  std::uint64_t part2;
};

Result
solve(const Input& input);
}
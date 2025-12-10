export module aoc25.day9;

import std;

export namespace aoc25::day9 {

struct Point
{
  std::uint64_t x;
  std::uint64_t y;

};

using Input = std::vector<Point>;

struct Result
{
  std::uint64_t part1;
  std::uint64_t part2;
};

Result
solve(const Input& input);
}
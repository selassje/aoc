
export module aoc25.day11;

import std;

export namespace aoc25::day11 {

struct Connection {
  std::string input;
  std::vector<std::string> outputs;
};


using Input = std::vector<Connection>;

struct Result
{
  std::uint64_t part1;
  std::uint64_t part2;
};

Result
solve(const Input& input);
}
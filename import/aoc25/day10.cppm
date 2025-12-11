export module aoc25.day10;

import std;

export namespace aoc25::day10 {

enum class Light: std::uint8_t
{
  Off = 0,
  On = 1,
};

using Lights =  std::vector<Light>;

using ButtonWiring = std::vector<std::uint64_t>;

using ButtonWirings = std::vector<ButtonWiring>;

using Joltages = std::vector<std::uint64_t>;


struct Machine
{
  Lights lights;
  ButtonWirings wirings;
  Joltages joltages;
};

using Input = std::vector<Machine>;

struct Result
{
  std::uint64_t part1;
  std::uint64_t part2;
};

Result
solve(const Input& input);
}
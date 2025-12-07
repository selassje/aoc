module;
#include <cstdint>
#include <vector>

export module aoc25.day7;

export namespace aoc25::day7 {

enum class Tile : std::uint8_t
{
  Empty = 0,
  Start,
  Splitter,
};

using Input = std::vector<std::vector<Tile>>;

struct Result
{
  std::uint64_t part1;
  std::uint64_t part2;
};

Result
solve(const Input& input);
}

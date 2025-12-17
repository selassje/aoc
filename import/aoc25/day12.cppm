export module aoc25.day12;

import std;

export namespace aoc25::day12 {

enum class Tile : std::uint8_t
{
  Empty,
  Full,
};

struct Shape
{
  std::uint64_t id;
  std::vector<std::vector<Tile>> grid;
};

using Shapes = std::vector<Shape>;

struct Region
{
  std::uint64_t width;
  std::uint64_t height;
  std::vector<std::uint64_t> shapesCount;
};

using Regions = std::vector<Region>;

struct Input
{
  Shapes shapes;
  Regions regions;
};

std::uint64_t
solve(const Input& input);
}
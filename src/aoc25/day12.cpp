module aoc25.day12;

import std;
import aoc.matrix;

using Matrix = aoc::matrix::Matrix<aoc25::day12::Tile>;
namespace aoc25::day12 {

Result
solve(const Input& input)
{
  auto shapes = std::vector<Matrix>{};
  for (std::size_t i = 0; i < input.shapes.size(); ++i) {
    shapes.emplace_back(input.shapes[i].grid, std::identity{});
    shapes.back().print([](auto tile) { return tile == Tile::Full ? '#' : '.'; });
    std::println();
  }
  const auto regions = input.regions;

  return { 0, 0 };
}
}
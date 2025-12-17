module aoc25.day12;

import std;
import aoc.matrix;

using ShapeGrid = aoc::matrix::Matrix<aoc25::day12::Tile>;
using ShapeGrids = std::vector<ShapeGrid>;
using Region = aoc25::day12::Region;
using Tile = aoc25::day12::Tile;

namespace {

bool
isRegionValid(const Region& region, const ShapeGrids& shapes)
{
  std::uint64_t tilesCount = 0;
  for (auto shapeIndex = 0UZ; shapeIndex < region.shapesCount.size();
       ++shapeIndex) {
    tilesCount +=
      shapes[shapeIndex].count(Tile::Full) * region.shapesCount[shapeIndex];
  }
  return tilesCount <= region.width * region.height;
}

}
namespace aoc25::day12 {

std::uint64_t
solve(const Input& input)
{
  auto shapes = std::vector<ShapeGrid>{};
  for (const auto& shape : input.shapes) {
    shapes.emplace_back(shape.grid, std::identity{});
  }
  std::uint64_t result = 0;

  for (const auto& region : input.regions) {
    result += isRegionValid(region, shapes) ? 1ULL : 0ULL;
  }
  return result;
}
}
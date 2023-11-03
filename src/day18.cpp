#include "day18.hpp"

#include <algorithm>
#include <array>
#include <queue>
#include <ranges>
#include <vector>


namespace aoc22::day18 {

struct CubeEx : public Cube
{
  constexpr CubeEx(const Cube& cube)
    : Cube(cube){};
  constexpr CubeEx(std::initializer_list<int> list)
  {
    auto it = list.begin();
    x = *it++;
    y = *it++;
    z = *it;
  }
  constexpr bool operator==(const CubeEx&) const noexcept = default;

  CubeEx operator+(const CubeEx& cube) const
  {
    return { x + cube.x, y + cube.y, z + cube.z };
  }
};

struct RectangularPrism
{
  CubeEx cornerMin;
  CubeEx cornerMax;

  bool contains(const CubeEx& cube) const
  {
    auto inRange = [](const auto& min, const auto& val, const auto& max) {
      return min <= val && val <= max;
    };
    return inRange(cornerMin.x, cube.x, cornerMax.x) &&
           inRange(cornerMin.y, cube.y, cornerMax.y) &&
           inRange(cornerMin.z, cube.z, cornerMax.z);
  }
};

using Cubes = std::vector<CubeEx>;

constexpr std::array<CubeEx, 6> SHIFTS = { CubeEx{ 1, 0, 0 }, { -1, 0, 0 },
                                           { 0, 1, 0 },       { 0, -1, 0 },
                                           { 0, 0, 1 },       { 0, 0, -1 } };

Cubes
getNeighbouringAirCubes(const CubeEx& cube, const Cubes& dropletCubes)
{
  Cubes airCubes{};
  airCubes.reserve(6);
  for (const auto& shift : SHIFTS) {
    const auto neighbour = cube + shift;
    if (std::ranges::find(dropletCubes, neighbour) == dropletCubes.end()) {
      airCubes.push_back(neighbour);
    }
  }
  return airCubes;
}

Result
solve(const Input& input)
{

  Cubes cubes(input.begin(), input.end());

  auto max = [&cubes](const auto& proj) {
    return std::ranges::max(cubes, {}, proj).*proj;
  };
  auto min = [&cubes](const auto& proj) {
    return std::ranges::min(cubes, {}, proj).*proj;
  };

  const RectangularPrism boundaries = {
    { min(&CubeEx::x), min(&CubeEx::y), min(&CubeEx::z) },
    { max(&CubeEx::x), max(&CubeEx::y), max(&CubeEx::z) }
  };

  auto airCubes = [&cubes](const auto& cube) {
      return getNeighbouringAirCubes(cube, cubes);
  };

  std::size_t part1 = 0;
  for (const auto& cube : cubes) {
    part1 += airCubes(cube).size();
  }
  return { part1, part1 };
}

}

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

using Cubes = std::vector<CubeEx>;

bool
contains(const Cubes& cubes, const CubeEx& cube)
{
  return std::ranges::find(cubes, cube) != cubes.end();
}

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
    if (!contains(dropletCubes, neighbour)) {
      airCubes.push_back(neighbour);
    }
  }
  return airCubes;
}

struct ScanResult
{
  bool isInternal;
  Cubes cubes;
};

ScanResult
scanForAirCubes(const CubeEx& cube,
                const Cubes& internalAirCubes,
                const Cubes& externalAirCubes)
{
  bool isInternal = true;
  Cubes scannedCubes{};
  return { isInternal, scannedCubes };
};

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

  auto getAirCubes = [&cubes](const auto& cube) {
    return getNeighbouringAirCubes(cube, cubes);
  };

  auto airCubesBounded = [&cubes, &boundaries](const auto& cube) {
    return getNeighbouringAirCubes(cube, cubes) |
           std::views::filter([&](const auto& innerCube) {
             return boundaries.contains(innerCube);
           });
  };

  Cubes cubesToBeSearched{};
  std::size_t part1 = 0;
  for (const auto& cube : cubes) {
    const auto airCubes = getAirCubes(cube);
    part1 += airCubes.size();
    for (const auto& airCube : airCubes) {
      if (boundaries.contains(airCube)) {
        cubesToBeSearched.push_back(airCube);
      }
    }
  }

  Cubes internalAirCubes{};
  Cubes externalAirCubes{};

  auto scan = [&](const auto& cube) {
    return scanForAirCubes(cube,internalAirCubes,externalAirCubes);
  };

  for (const auto& cube : cubesToBeSearched) {
    if (!contains(internalAirCubes, cube) &&
        !contains(externalAirCubes, cube)) {
        const auto scanResult = scan(cube);
    }
  }

  std::size_t part2 = 0;
  for (const auto& cube : cubes) {
    const auto airCubes = getAirCubes(cube);
    for (const auto& airCube : airCubes) {
      if (!contains(internalAirCubes, airCube)) {
        ++part2;
      }
    }
  }
  return { part1, part2 };
}

}

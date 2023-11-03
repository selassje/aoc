#include "day18.hpp"

#include <algorithm>
#include <array>
#include <iterator>
#include <queue>
#include <ranges>
#include <vector>

namespace aoc22::day18 {

struct CubeEx
{
  std::int32_t x;
  std::int32_t y;
  std::int32_t z;
  
  constexpr CubeEx(const Cube& cube)
    : x{cube.x}, y{cube.y}, z{cube.z} {};
  
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
struct Boundaries
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
                const Cubes& dropletCubes,
                const Boundaries& boundaries)
{
  auto getAirCubes = [&dropletCubes](const auto& cube) {
    return getNeighbouringAirCubes(cube, dropletCubes);
  };
  bool isInternal = true;
  Cubes scannedCubes{};
  std::queue<CubeEx> toBeScanned{};
  toBeScanned.push(cube);
  while(!toBeScanned.empty()) {
      const auto current = toBeScanned.back();
      for(const auto& neighbourAirCube : getAirCubes(current)) {
          if (!boundaries.contains(neighbourAirCube)) {
            isInternal = false;
          } else if(!contains(scannedCubes, neighbourAirCube)) {
            toBeScanned.push(neighbourAirCube);
          }
      }
      scannedCubes.push_back(current);
      toBeScanned.pop();
  }
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

  const Boundaries boundaries = {
    { min(&CubeEx::x), min(&CubeEx::y), min(&CubeEx::z) },
    { max(&CubeEx::x), max(&CubeEx::y), max(&CubeEx::z) }
  };

  auto getAirCubes = [&cubes](const auto& cube) {
    return getNeighbouringAirCubes(cube, cubes);
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
    return scanForAirCubes(cube, cubes, boundaries);
  };

  for (const auto& cube : cubesToBeSearched) {
    if (!contains(internalAirCubes, cube) &&
        !contains(externalAirCubes, cube)) {
      const auto scanResult = scan(cube);
      auto& cubesToAppendTo =
        scanResult.isInternal ? internalAirCubes : externalAirCubes;
      std::ranges::copy(scanResult.cubes, std::back_inserter(cubesToAppendTo));
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

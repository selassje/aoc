#include "aoc22/day18.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <iterator>
#include <queue>
#include <vector>

namespace aoc22::day18 {

struct CubeEx
{
  std::int32_t x;
  std::int32_t y;
  std::int32_t z;

  explicit constexpr CubeEx(const Cube& cube)
    : x{ cube.x }
    , y{ cube.y }
    , z{ cube.z } {};

  constexpr CubeEx(std::initializer_list<int> list)
  {
    auto it = list.begin(); // NOLINT
    x = *it++;
    y = *it++;
    z = *it;
  }
  constexpr bool operator==(const CubeEx&) const noexcept = default;

  [[nodiscard]] friend CubeEx operator+(const CubeEx& lhs, const CubeEx& rhs)
  {
    return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
  }
};

struct Cubes : public std::vector<CubeEx>
{
  using Base = std::vector<CubeEx>;
  using Iterator = Input::const_iterator;

  Cubes() = default;
  Cubes(Iterator begin, Iterator end)
    : Base(begin, end)
  {
  }

  [[nodiscard]] bool contains(const CubeEx& cube) const
  {
    return std::ranges::find(*this, cube) != end();
  }
};

struct Boundaries
{
  CubeEx cornerMin;
  CubeEx cornerMax;

  [[nodiscard]] bool contains(const CubeEx& cube) const
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

namespace {

Cubes
getNeighbouringAirCubes(const CubeEx& cube, const Cubes& dropletCubes)
{
  Cubes airCubes{};
  airCubes.reserve(SHIFTS.max_size());
  for (const auto& shift : SHIFTS) {
    const auto neighbour = cube + shift;
    if (!dropletCubes.contains(neighbour)) {
      airCubes.push_back(neighbour);
    }
  }
  return airCubes;
}

}
struct ScanResult
{
  bool isInternal{};
  Cubes cubes;
};

namespace {

ScanResult
scanForAirCubes(const CubeEx& cube,
                const Cubes& dropletCubes,
                const Boundaries& boundaries)
{
  auto getAirCubes = [&dropletCubes](const auto& cubeLambda) {
    return getNeighbouringAirCubes(cubeLambda, dropletCubes);
  };
  bool isInternal = true;
  Cubes scannedCubes{};
  std::queue<CubeEx> toBeScannedCubes{};
  toBeScannedCubes.push(cube);
  while (!toBeScannedCubes.empty()) {
    const auto currentCube = toBeScannedCubes.back();
    for (const auto& neighbourAirCube : getAirCubes(currentCube)) {
      if (!boundaries.contains(neighbourAirCube)) {
        isInternal = false;
      } else if (!scannedCubes.contains(neighbourAirCube)) {
        toBeScannedCubes.push(neighbourAirCube);
      }
    }
    scannedCubes.push_back(currentCube);
    toBeScannedCubes.pop();
  }
  return { isInternal, scannedCubes };
};
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

  const Boundaries boundaries = {
    { min(&CubeEx::x), min(&CubeEx::y), min(&CubeEx::z) },
    { max(&CubeEx::x), max(&CubeEx::y), max(&CubeEx::z) }
  };

  auto getAirCubes = [&cubes](const auto& cube) {
    return getNeighbouringAirCubes(cube, cubes);
  };

  Cubes adjacentAirCubes{};
  std::size_t part1 = 0;
  for (const auto& cube : cubes) {
    const auto airCubes = getAirCubes(cube);
    part1 += airCubes.size();
    std::ranges::copy(airCubes, std::back_inserter(adjacentAirCubes));
  }

  Cubes internalAirCubes{};
  Cubes externalAirCubes{};

  auto scan = [&](const auto& cube) {
    return scanForAirCubes(cube, cubes, boundaries);
  };

  for (const auto& cube : adjacentAirCubes) {
    if (!internalAirCubes.contains(cube) && !externalAirCubes.contains(cube)) {
      const auto scanResult = scan(cube);
      auto& cubesToAppendTo =
        scanResult.isInternal ? internalAirCubes : externalAirCubes;
      std::ranges::copy(scanResult.cubes, std::back_inserter(cubesToAppendTo));
    }
  }

  const auto part2 = static_cast<std::size_t>(
    std::ranges::count_if(adjacentAirCubes, [&](const auto& cube) {
      return !internalAirCubes.contains(cube);
    }));

  return { part1, part2 };
}

}

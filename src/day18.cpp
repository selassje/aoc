#include "day18.hpp"

#include <algorithm>
#include <array>
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
  constexpr auto operator<=>(const CubeEx&) const noexcept = default;

  CubeEx operator+(const CubeEx& cube) const
  {
    return { x + cube.x, y + cube.y, z + cube.z };
  }
};

constexpr std::array<CubeEx, 6> SHIFTS = { CubeEx{ 1, 0, 0 }, { -1, 0, 0 },
                                           { 0, 1, 0 },       { 0, -1, 0 },
                                           { 0, 0, 1 },       { 0, 0, -1 } };

Result
solve(const Input& input)
{
  std::vector<CubeEx> cubes(input.begin(), input.end());
  std::size_t part1 = 0;
  for (const auto& cube : cubes) {
    for (const auto& shift : SHIFTS) {
      const auto shifted = cube + shift;
      if (std::ranges::find(cubes, shifted) == cubes.end()) {
        ++part1;
      }
    }
  }
  return { part1, part1 };
}

}

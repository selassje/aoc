#include "aoc24/day9.hpp"
#include <cstddef>
#include <variant>
#include <vector>

namespace aoc24::day9 {

namespace {

struct File
{
  std::size_t id;
  std::size_t size;
};

struct Empty
{
  std::size_t size;
};

using DiskElement = std::variant<File, Empty>;

using DiskMap = std::vector<DiskElement>;

auto
getDiskMap(Input input)
{
  DiskMap map{};
  std::size_t id = 0;
  for (std::size_t i = 0; i < input.size(); ++i) {
    if (i % 2 == 0) {
      map.emplace_back(File{ id, static_cast<std::size_t>(input[i]) });
      ++id;
    } else {
      map.emplace_back(Empty{ static_cast<std::size_t>(input[i]) });
    }
  }
  return map;
}

}

Result
solve(Input input)
{
  auto diskMap =  getDiskMap(input);

  const std::size_t part1 = input.size();
  const std::size_t part2 = input.size();
  return { part1, part2 };
}

}
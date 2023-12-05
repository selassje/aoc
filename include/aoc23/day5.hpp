
#ifndef AOC23_DAY5_HPP
#define AOC23_DAY5_HPP

#include <cstddef>
#include <vector>

namespace aoc23::day5 {

struct Range
{
  std::size_t source;
  std::size_t destination;
  std::size_t length;
};

using Map = std::vector<Range>;
using Seeds = std::vector<std::size_t>;

struct Input
{
  Seeds seeds;
  Map seedToSoil;
  Map soilToFertilizer;
  Map fertilizerToWater;
  Map waterToLight;
  Map lightToTemp;
  Map tempToHumidity;
  Map humidityToToLocation;
};

struct Result
{
  std::size_t part1;
  std::size_t part2;
};

Result
solve(const Input& input);
}

#endif
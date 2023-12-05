#include "aoc23/day5.hpp"

#include <cstddef>
#include <limits>

namespace aoc23::day5 {

std::size_t
getDestination(std::size_t source, const Map& map)
{
  for (const auto& range : map) {
    if (range.source <= source && source <= range.source + range.length - 1) {
      return range.destination + source - range.source;
    }
  }
  return source;
};

Result
solve(const Input& input)
{
  std::size_t part1 = std::numeric_limits<std::size_t>::max();
  for (const auto& seed : input.seeds) {
   const auto soil = getDestination(seed, input.seedToSoil);
    const auto fertilizer = getDestination(soil, input.soilToFertilizer);
    const auto water = getDestination(fertilizer, input.fertilizerToWater);
    const auto light = getDestination(water, input.waterToLight);
    const auto temp = getDestination(light, input.lightToTemp);
    const auto humidity = getDestination(temp, input.tempToHumidity);
    const auto location = getDestination(humidity, input.humidityToToLocation);
    part1 = std::min(part1, location);
  }
  return { part1, part1 };
}

}
#include "aoc23/day5.hpp"

#include <array>
#include <cstddef>
#include <limits>

namespace aoc23::day5 {

struct Destination
{
  std::size_t destination;
  std::size_t remainingRange;
};

using MapArray = std::array<Map, 7>;

Destination
getDestination(std::size_t source, const Map& map)
{
  auto remainingRange = std::numeric_limits<std::size_t>::max();
  bool nextRangeFound = false;
  for (const auto& range : map) {
    if (range.source <= source && source <= range.source + range.length - 1) {
      const auto destination = range.destination + source - range.source;
      remainingRange = range.length - (destination - range.destination + 1);
      return { destination, remainingRange };
    } else if (!nextRangeFound && range.source > source) {
      nextRangeFound = true;
      remainingRange = range.source - source - 1;
    }
  }
  return { source, remainingRange };
};

Destination
getSeedLocation(std::size_t seed, const Input& input)
{
  const auto [soil, soilRange] = getDestination(seed, input.seedToSoil);
  const auto [fertilizer, fertilizerRange] =
    getDestination(soil, input.soilToFertilizer);
  const auto [water, waterRange] =
    getDestination(fertilizer, input.fertilizerToWater);
  const auto [light, lightRange] = getDestination(water, input.waterToLight);
  const auto [temp, tempRange] = getDestination(light, input.lightToTemp);
  const auto [humidity, humidityRange] =
    getDestination(temp, input.tempToHumidity);
  const auto [location, locationRange] =
    getDestination(humidity, input.humidityToLocation);
  const auto minRange = std::min({ soilRange,
                                   fertilizerRange,
                                   waterRange,
                                   lightRange,
                                   tempRange,
                                   humidityRange,
                                   locationRange });
  return { location, minRange };
}

Result
solve(const Input& input)
{
  const MapArray maps{ input.seedToSoil,          input.soilToFertilizer,
                       input.fertilizerToWater,   input.waterToLight,
                       input.lightToTemp,         input.tempToHumidity,
                       input.humidityToLocation };

  auto getLocationAndMinLength = [&maps](const std::size_t seed ) -> Destination {
    std::size_t minLength = std::numeric_limits<std::size_t>::max();
    std::size_t location = seed;
    for ( const auto& map : maps) {
        const auto destination = getDestination(location,map);
        location = destination.destination;
        minLength = std::min(minLength,destination.remainingRange);
    }
    return {location, minLength};
  };

  std::size_t part1 = std::numeric_limits<std::size_t>::max();
  std::size_t part2 = std::numeric_limits<std::size_t>::max();
  for (std::size_t i = 0; i < input.seeds.size(); ++i) {
    auto seed = input.seeds[i];
    part1 = std::min(part1, getLocationAndMinLength(seed).destination);
    if ((i + 1) % 2 == 0) {
      seed = input.seeds[i - 1];
      const auto endSeed = seed + input.seeds[i] - 1;
      while (seed <= endSeed) {
        auto location = getLocationAndMinLength(seed);
        part2 = std::min(part2, location.destination);
        seed += location.remainingRange + 1;
      }
    }
  }
  return { part1, part2 };
}

}
#include "aoc23/day5.hpp"

#include <array>
#include <cstddef>
#include <limits>
#include <optional>

namespace aoc23::day5 {

struct DestinationAndRemainingLengthInTheCurrentRange
{
  std::size_t destination;
  std::size_t remainingLength;
};

using MapArray = std::array<Map, 7>;

DestinationAndRemainingLengthInTheCurrentRange
getDestinationAndRemainingLength(std::size_t source, const Map& map)
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


Result
solve(const Input& input)
{
  const MapArray maps{ input.seedToSoil,          input.soilToFertilizer,
                       input.fertilizerToWater,   input.waterToLight,
                       input.lightToTemp,         input.tempToHumidity,
                       input.humidityToLocation };
  
  struct LocationAndMinLength {
    std::size_t location;
    std::size_t minLength;
  };

  auto getLocationAndMinLength = [&maps](const std::size_t seed ) -> LocationAndMinLength {
    std::size_t minLength = std::numeric_limits<std::size_t>::max();
    std::size_t location = seed;
    for ( const auto& map : maps) {
        const auto destination = getDestinationAndRemainingLength(location,map);
        location = destination.destination;
        minLength = std::min(minLength,destination.remainingLength);
    }
    return {location, minLength};
  };

  std::size_t part1 = std::numeric_limits<std::size_t>::max();
  std::size_t part2 = std::numeric_limits<std::size_t>::max();
  for (std::size_t i = 0; i < input.seeds.size(); ++i) {
    auto seed = input.seeds[i];
    part1 = std::min(part1, getLocationAndMinLength(seed).location);
    if ((i + 1) % 2 == 0) {
      seed = input.seeds[i - 1];
      const auto endSeed = seed + input.seeds[i] - 1;
      while (seed <= endSeed) {
        auto location = getLocationAndMinLength(seed);
        part2 = std::min(part2, location.location);
        seed += location.minLength + 1;
      }
    }
  }
  return { part1, part2 };
}

}
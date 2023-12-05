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
  std::optional<std::size_t> remainingLength{};
  for (const auto& range : map) {
    if (range.source <= source && source <= range.source + range.length - 1) {
      const auto destination = range.destination + source - range.source;
      remainingLength = range.length - (destination - range.destination + 1);
      return { destination, *remainingLength };
    } else if (!remainingLength && range.source > source) {
      remainingLength = range.source - source - 1;
    }
  }
  return { source,
           remainingLength.value_or(std::numeric_limits<std::size_t>::max()) };
};

Result
solve(const Input& input)
{
  const MapArray maps{ input.seedToSoil,        input.soilToFertilizer,
                       input.fertilizerToWater, input.waterToLight,
                       input.lightToTemp,       input.tempToHumidity,
                       input.humidityToLocation };

  struct LocationAndNextSeed
  {
    std::size_t location;
    std::size_t nextSeed;
  };

  auto getLocationAndNextSeed =
    [&maps](const std::size_t seed) -> LocationAndNextSeed {
    std::size_t minLength = std::numeric_limits<std::size_t>::max();
    std::size_t location = seed;
    for (const auto& map : maps) {
      const auto destination = getDestinationAndRemainingLength(location, map);
      location = destination.destination;
      minLength = std::min(minLength, destination.remainingLength);
    }
    return { location, seed + minLength + 1 };
  };

  std::size_t part1 = std::numeric_limits<std::size_t>::max();
  std::size_t part2 = std::numeric_limits<std::size_t>::max();
  for (std::size_t i = 0; i < input.seeds.size(); ++i) {
    auto seed = input.seeds[i];
    part1 = std::min(part1, getLocationAndNextSeed(seed).location);
    if ((i + 1) % 2 == 0) {
      seed = input.seeds[i - 1];
      const auto endSeed = seed + input.seeds[i] - 1;
      while (seed <= endSeed) {
        const auto& [location, nextSeed] = getLocationAndNextSeed(seed);
        part2 = std::min(part2, location);
        seed = nextSeed;
      }
    }
  }
  return { part1, part2 };
}

}
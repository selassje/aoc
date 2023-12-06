#include "aoc23/day6.hpp"

#include <string>

namespace aoc23::day6 {

std::size_t
recordBeatingCount(const Race& race)
{
  std::size_t result = 0;
  for (std::size_t t = 0; t <= race.time; ++t) {
    const auto distance = t * (race.time - t);
    if (distance > race.recordDistance) {
      ++result;
    }
  }
  return result;
}

Result
solve(const Input& input)
{
  std::size_t part1 = 1;
  std::string singleRaceRecordDistanceStr{};
  std::string singleRaceTimeStr{};
  for (const auto& race : input) {
    part1 *= recordBeatingCount(race);
    singleRaceTimeStr += std::to_string(race.time);
    singleRaceRecordDistanceStr += std::to_string(race.recordDistance);
  }

  auto toInt = [](const std::string& string) {
    return std::strtoull(string.c_str(), nullptr, 10);
  };

  Race singleRace{ toInt(singleRaceRecordDistanceStr),
                   toInt(singleRaceTimeStr) };
  const std::size_t part2 = recordBeatingCount(singleRace);

  return { part1, part2 };
}
}
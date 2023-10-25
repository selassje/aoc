#include "day16.hpp"

#include <algorithm>
#include <array>
#include <deque>
#include <map>
#include <numeric>
#include <optional>
#include <ranges>
#include <set>
#include <string>
#include <vector>

namespace aoc22::day16 {

constexpr std::size_t
valveToIndex(std::string_view name)
{
  const std::size_t divider = 'Z' - 'A' + 1;
  auto toIndex = [](const char c) { return static_cast<std::size_t>(c - 'A'); };
  return toIndex(name[0]) * divider + toIndex(name[1]);
};

constexpr std::string
indexToName(std::size_t index)
{
  const std::size_t divider = 'Z' - 'A' + 1;
  const char second = static_cast<char>(index / divider) + 'A';
  const char first = static_cast<char>(index % divider) + 'A';
  return std::string{ second, first };
};


using DistanceMap = std::map<std::size_t, std::map<std::size_t, std::size_t>>;

Valve&
getValve(std::string_view name, Input& input)
{
  const auto it = std::ranges::find(input, name, &Valve::name);
  return *it;
}

const Valve&
getValve(std::string_view name, const Input& input)
{
  return *std::ranges::find(input, name, &Valve::name);
}

std::size_t
maxPressureReleasePart1(std::size_t valve,
                        std::size_t minutes,
                        const std::set<std::size_t>& openedValves,
                        const DistanceMap& distMap,
                        const Input& input)
{
  std::vector<std::size_t> results{ 0 };
  auto openedValves_ = openedValves;
  openedValves_.insert(valve);
  const auto& distances = distMap.at(valve);
  for (const auto& [k, d] : distances) {
    const auto& valveObj = getValve(indexToName(k), input);
    if (k != valve && !openedValves.contains(k) && valveObj.flowRate > 0) {
      const auto minutesSpent = d + 1;
      if (minutes > minutesSpent) {
        const auto result =
          valveObj.flowRate * (minutes - minutesSpent) +
          maxPressureReleasePart1(
            k, minutes - minutesSpent, openedValves_, distMap, input);
        results.push_back(result);
      }
    }
  }
  return std::ranges::max(results);
}
std::size_t
maxPressureReleasePart2(std::size_t valve1,
                        std::size_t valve2,
                        std::size_t minutes1,
                        std::size_t minutes2,
                        const std::set<std::size_t>& openedValves,
                        const DistanceMap& distMap,
                        const Input& input)
{
  std::vector<std::size_t> results{ 0 };
  auto openedValves_ = openedValves;
  openedValves_.insert(valve1);
  openedValves_.insert(valve2);
  const auto& distances1 = distMap.at(valve1);
  const auto& distances2 = distMap.at(valve2);
  for (const auto& [k, d] : distances1) {
    for (const auto& [k2, d2] : distances2) {

      const auto flowRate1 = getValve(indexToName(k), input).flowRate;
      const auto flowRate2 = getValve(indexToName(k2), input).flowRate;
      if (k != valve1 && k2 != valve2 && flowRate1 > 0 && flowRate2 > 0) {

        const auto minutesSpent1 = d + 1;
        const auto minutesSpent2 = d2 + 1;
        std::size_t result1 = 0;
        std::size_t result2 = 0;
        if (k != valve1 && !openedValves_.contains(k) && flowRate1 > 0) {
          if (minutes1 > minutesSpent1) {
            result1 = flowRate1 * (minutes1 - minutesSpent1);
          }
        }
        if (k2 != valve2 && !openedValves_.contains(k2) && flowRate2 > 0) {
          if (minutes2 > minutesSpent2) {
            result2 = flowRate2 * (minutes2 - minutesSpent2);
          }
        }
        auto str1 = indexToName(k);
        auto str2 = indexToName(k2);
        const auto totalRes = result1 + result2;
        if (result1 != 0 && result2 != 0) {
          auto result = maxPressureReleasePart2(k,
                                                k2,
                                                minutes1 - minutesSpent1,
                                                minutes2 - minutesSpent2,
                                                openedValves_,
                                                distMap,
                                                input);
          if (k != k2) {
            result += result2 + result1;
          } else {
            result += std::max({ result1, result2 });
          }
          results.push_back(result);
        } else {
          int z = 3;
        }
      }
    }
  }
  return std::ranges::max(results);
}

auto
findShortestPath(std::size_t src, const Input& input)
{
  static constexpr auto maxDistance =
    std::numeric_limits<decltype(Result::part1)>::max();

  std::set<std::size_t> visitedValves{};
  std::deque<std::size_t> toBeVisitedValves{};
  std::map<std::size_t, std::size_t> distances{};

  for (const auto& valve : input) {
    const auto valveIndex = valveToIndex(valve.name);
    if (valveIndex == src) {
      distances[valveIndex] = 0;
    } else {
      distances[valveIndex] = maxDistance;
    }
  }

  toBeVisitedValves.push_back(src);
  while (!toBeVisitedValves.empty()) {
    const auto valveIndex = toBeVisitedValves.front();
    const auto& valve = getValve(indexToName(valveIndex), input);
    for (const auto& neighbour : valve.connectedValves) {
      const auto neighbourIndex = valveToIndex(neighbour);
      if (!visitedValves.contains(neighbourIndex)) {
        const auto neighbourValve = getValve(neighbour, input);
        if (const auto canidateLength = distances[valveIndex] + 1;
            canidateLength < distances[neighbourIndex]) {
          distances[neighbourIndex] = canidateLength;
        }
        toBeVisitedValves.push_back(neighbourIndex);
      }
    }
    visitedValves.insert(valveIndex);
    toBeVisitedValves.pop_front();
  }
  return distances;
}

Result
solve(const Input& input)
{
  std::map<std::size_t, std::map<std::size_t, std::size_t>> distances;
  for (const auto& valve : input) {
    const auto valveIndex = valveToIndex(valve.name);
    if (valveIndex == 0 || valve.flowRate > 0) {
      distances[valveIndex] = findShortestPath(valveIndex, input);
    }
  }

  std::size_t part1 = 0;
  part1 = maxPressureReleasePart1(0, 30, {}, distances, input);

  std::size_t part2 = 0;
  part2 = maxPressureReleasePart2(0, 0, 26, 26, {}, distances, input);

  return { part1, part2 };
}
};
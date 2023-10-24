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

constexpr auto maxValves = valveToIndex("ZZ") + 1;
constexpr std::size_t totalMinutes = 30;
constexpr std::size_t actionMove = 0;
constexpr std::size_t actionOpen = 1;
constexpr std::size_t actionNone = 2;

using Dp = std::optional<std::size_t>[totalMinutes][maxValves][actionNone + 1];

using DistMap = std::map<std::size_t, std::map<std::size_t, std::size_t>>;

Valve&
getValve(std::string_view name, Input& input)
{
  const auto it = std::ranges::find(input, name, &Valve::name);
  return *it;
}
bool
isValve(std::string_view name, const Input& input)
{
  const auto it = std::ranges::find(input, name, &Valve::name);
  return it != input.end();
}

const Valve&
getValve(std::string_view name, const Input& input)
{
  return *std::ranges::find(input, name, &Valve::name);
}

std::size_t
maxPressureRelease(std::size_t valve,
                   std::size_t minutes,
                   const std::set<std::size_t>& openedValves,
                   const DistMap& distMap,
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
          maxPressureRelease(
            k, minutes - minutesSpent, openedValves_, distMap, input);
        results.push_back(result);
      }
    }
  }
  return std::ranges::max(results);
}
std::size_t
maxPressureRelease2(std::size_t valve1,
                    std::size_t valve2,
                    std::size_t minutes1,
                    std::size_t minutes2,
                    const std::set<std::size_t>& openedValves,
                    const DistMap& distMap,
                    const Input& input)
{
  std::vector<std::size_t> results{ 0 };
  auto openedValves_ = openedValves;
  openedValves_.insert(valve1);
  openedValves_.insert(valve2);
  const auto& distances1 = distMap.at(valve1);
  const auto& distances2 = distMap.at(valve2);
  for (const auto& [k, d] : distances1) {
    for (const auto& [k2, d2] : distances2) 
    {
      //const auto k2 = k;
      //const auto d2 = d;

      const auto flowRate1 = getValve(indexToName(k), input).flowRate;
      const auto flowRate2 = getValve(indexToName(k2), input).flowRate;
      const auto minutesSpent1 = d + 1;
      const auto minutesSpent2 = d2 + 1;
      std::size_t result1 = 0;
      std::size_t result2 = 0;
      if (k != valve1 && !openedValves.contains(k) && flowRate1 > 0) {
        if (minutes1 > minutesSpent1) {
          result1 = flowRate1 * (minutes1 - minutesSpent1);
        }
      }
      if (k2 != valve2 && !openedValves.contains(k2) && flowRate2 > 0) {
        if (minutes2 > minutesSpent2) {
          result2 = flowRate2 * (minutes2 - minutesSpent2);
        }
      }
      if (result1 != 0 && result2 != 0) {
        auto result = result1 + maxPressureRelease2(k,
                                                    k2,
                                                    minutes1 - minutesSpent1,
                                                    minutes2 - minutesSpent2,
                                                    openedValves_,
                                                    distMap,
                                                    input);
        if (k != k2) {
          result += result2;
        }
        results.push_back(result);
      } else {
        int z = 3;
      }
    }
  }
  return std::ranges::max(results);
}

std::size_t
getDp(Dp& dp, std::size_t m, std::size_t v, std::size_t a)
{
  const auto d = dp[m][v][a];
  return d ? *d : 0;
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
    distances[valveIndex] = findShortestPath(valveIndex, input);
  }

  const auto nonZeroValves = static_cast<std::size_t>(std::ranges::count_if(
    input, [](const auto& valve) { return valve.flowRate > 0; }));

  std::size_t part1 = 0;

  part1 = maxPressureRelease(0, 30, {}, distances, input);

  std::size_t part2 = 0;
  part2 = maxPressureRelease2(0, 0, 26, 26, {}, distances, input);

  // const auto& distancesFromFirst = distances[0];
  // for (const auto& [k1, d1] : distancesFromFirst) {
  //   for (const auto& [k2, d2] : distancesFromFirst) {
  //     const auto& valveObj1 = getValve(indexToName(k1), input);
  //     const auto& valveObj2 = getValve(indexToName(k2), input);
  //    std::vector<std::size_t> results{0};
  //    if (k1 != 0 && k2 != 0 && k1 != k2 && valveObj1.flowRate > 0 &&
  //       valveObj2.flowRate > 0) {
  //        const auto result1 = maxPressureRelease(k1,24,{},distances,input) +
  //        24*valveObj1.flowRate;
  //       const auto result2 = maxPressureRelease(k2,24,{},distances,input) +
  //       24*valveObj1.flowRate;
  // }
  // part2 = std::max(part1,results);
  // }
  //}

  return { part1, part2 };

  auto currentValve = 0;
  auto minutesLeft = static_cast<int>(totalMinutes);
  std::set<std::size_t> openedValves{};

  while (openedValves.size() < nonZeroValves && minutesLeft > 0) {
    const auto distancesFromCurrent = distances[currentValve];
    const auto strCurr = indexToName(currentValve);
    std::size_t maxFlow = 0;
    std::size_t nextValve = 0;
    std::size_t maxMinutesSpent = 0;
    for (const auto& [k, d] : distancesFromCurrent) {
      if (k != currentValve && !openedValves.contains(k)) {
        const auto minutesSpent = d + 1;
        const auto flowRate = getValve(indexToName(k), input).flowRate;
        if (minutesLeft - minutesSpent > 0) {
          const auto pressure =
            static_cast<std::size_t>(minutesLeft - minutesSpent) * flowRate;
          if (pressure > maxFlow) {
            nextValve = k;
            maxFlow = pressure;
            maxMinutesSpent = minutesSpent;
          }
        }
      }
    }
    if (maxFlow != 0) {
      const auto str = indexToName(nextValve);
      currentValve = nextValve;
      minutesLeft -= maxMinutesSpent;
      part1 += maxFlow;
      openedValves.insert(nextValve);
    } else {
      break;
    }
  }
  return { part1, part1 };
}
Result
solve2(const Input& input)
{
  Dp dp{};
  dp[0][0][actionOpen] = 0;
  for (const auto& valve : getValve("AA", input).connectedValves) {
    dp[0][valveToIndex(valve)][actionMove] = 0;
  }

  for (std::size_t minute = 0; minute < totalMinutes; ++minute) {
    for (std::size_t valve = 0; valve < maxValves; ++valve) {
      for (std::size_t action = 0; action <= actionNone; ++action) {
        auto& d = dp[minute][valve][action];
        if (minute == 0) {
          d = 0;
        } else if (isValve(indexToName(valve), input)) {

          if (action == actionNone) {
            d = std::max({ getDp(dp, minute - 1, valve, actionMove),
                           getDp(dp, minute - 1, valve, actionOpen),
                           getDp(dp, minute - 1, valve, actionNone) });
          }
          if (action == actionOpen) {
            try {
              d = std::max({ getDp(dp, minute - 1, valve, actionMove),
                             getDp(dp, minute - 1, valve, actionOpen),
                             getDp(dp, minute - 1, valve, actionNone) }) +
                  getValve(indexToName(valve), input).flowRate *
                    (totalMinutes - minute - 1);
              auto t = d;
              auto str = indexToName(valve);
              auto x = t;
            } catch (...) {
              d = 0;
            }
          }
          if (action == actionMove) {
            const auto prev =
              std::max({ getDp(dp, minute - 1, valve, actionMove),
                         getDp(dp, minute - 1, valve, actionOpen),
                         getDp(dp, minute - 1, valve, actionNone) });
          }
        }
      }
    }
  }
  std::size_t part1 = 0;
  //  part1 = maxPressureRelease("AA", 30, input);
  for (std::size_t minute = 0; minute < totalMinutes; ++minute) {
    for (std::size_t valve = 0; valve < maxValves; ++valve) {
      const auto d = dp[minute][valve][actionOpen];
      if (d) {
        //      part1 = std::max(part1, *d);
      }
    }
  }

  return { part1, part1 };
}
};
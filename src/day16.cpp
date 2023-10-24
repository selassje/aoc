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
          maxPressureRelease(k, minutes - minutesSpent, openedValves_, distMap, input);
        results.push_back(result);
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

  part1 = maxPressureRelease(0,30,{},distances,input);
  return {part1,part1};


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